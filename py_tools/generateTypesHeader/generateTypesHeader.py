# paciFIST studios. 2025. MIT License

import argparse
import datetime
import itertools
import os
from email.iterators import typed_subpart_iterator
from pathlib import Path
import re
import sys
from urllib.parse import ResultBase
from xml.dom import WrongDocumentErr

""" The project root is 3 directories up from this file """
PACIFIST_ROOT = Path(__file__).parents[2]

""" An 80 character string of dashes, used to provide a visual separation in the logs """
DEFAULT_LOG_LINE = '-' * 80

""" A return code indicating no error was encountered when running this program """
EXIT_NO_ERROR = 0

""" A return code indicating an error was encountered when running this program  """
EXIT_WITH_ERROR = 1

""" The name of this script, as a string, which is used to name the logger """
PROGRAM_NAME_STRING = os.path.basename(__file__)



# Utilities ------------------------------------------------------------------------------------------------------------

def read_file_as_string(file_path: str, encoding: str = 'utf-8') -> str:
    if os.path.isfile(file_path):
        with open(file_path, 'r', encoding=encoding) as file:
            return file.read()
    return ''

def discover_source_file_paths(root_path: str) -> list:
    if not os.path.isdir(root_path):
        return []

    def _get_ext(f) -> str:
        """ Splits a string using periods, and then returns the last portion of the results.
         If this is a file path with an extention, it should return just the extension"""
        return f.split('.')[-1]

    def _allowed_exts() -> list:
        """ Returns an array of valid extensions, for files which are allowed to declare type symbols.
            In C, this (I think) is just a body file and a header file. 
         """
        return ['c', 'h']

    results = []
    
    for current, subdirs, files in os.walk(root_path):
        for file in files:
                ext = _get_ext(file)
                
                if ext in _allowed_exts():
                    file_path = os.path.join(current, file)
                    results.append(file_path)
   
        for subdir in subdirs:
            subdir_path = os.path.join(current, subdir)
            subdir_files = discover_source_file_paths(subdir_path)
            for file in subdir_files:
                ext = _get_ext(file)
                if ext in _allowed_exts():
                    results.append(file)
    
    return results
    
    
def parse_files_for_type_symbols(file_paths: list) -> list:
    if not file_paths:
        return []

    default_types = [
        'char_t',
        'char_ptr_t',
        'char_const_ptr_t',
        'void_t',
        'void_ptr_t',
        'void_const_ptr_t',
    ] 

    type_symbol_results = []

    # NOTE: for this to work, we have to use the following:
    #
    #   typedef enum EMyEnum_t { 
    #       ESomeValue1,
    #       ESomeValue2
    #   } EMyEnum_t;
    #
    #   typedef struct MyStruct_t {
    #       int32 value1;
    #       int32 value2;
    #   } MyStruct_t;
    #
    #   # this one is for MemoryIndex_t
    #   typedef size_t *_t
    #
    regex_data_type_match_pattern = 'typedef * (.*_t)'
    regex_data_type_matcher = re.compile(regex_data_type_match_pattern)

    regex_fn_type_pattern = '(?:[\(])(.*Function_t)(?:[\)])\w*(?:\()'
    regex_fn_type_matcher = re.compile(regex_fn_type_pattern)

    for file in file_paths:
        if not os.path.isfile(file):
            continue
            
        file_string = read_file_as_string(file)
        
        date_type_matches = regex_data_type_matcher.findall(file_string)
        if date_type_matches:
            for match in date_type_matches:
                pruned = match.split('struct')[-1].strip()
                pruned = pruned.split('union')[-1].strip()
                pruned = pruned.split('enum')[-1].strip()
                pruned = pruned.split('size_t')[-1].strip()
               
                if pruned == '':
                    continue
                
                if pruned not in type_symbol_results:
                    type_symbol_results.append(pruned)
                    continue

        fn_type_matches = regex_fn_type_matcher.findall(file_string)
        if fn_type_matches:
            for fn_type_matches in fn_type_matches:
                pruned = fn_type_matches.strip()
               
                if pruned == '':
                    continue
                
                if pruned not in type_symbol_results:
                    type_symbol_results.append(pruned)
                    continue
         
         
    # also add a ptr, a ptr-to-const version for all types     
    def _build_expanded_symbol_types(types):
        result = []
        
        for t in types:
            base = t.split('_t')[0]
            base_type = t
            ptr_type = f'{base}_ptr_t'
            const_ptr_type = f'{base}_const_ptr_t' 
            result.append(base_type)
            result.append(ptr_type)
            result.append(const_ptr_type)
        
        return result
   
    type_symbol_results = _build_expanded_symbol_types(type_symbol_results)
    for dt in default_types:
        type_symbol_results.append(dt)
    
     
    type_symbol_results.sort()
    type_symbol_results.insert(0, 'Undefined_t')
    return type_symbol_results


# todo: add some tests for this, even though it's working right now
# note: also works for pascal case
def split_camel_case_string(string: str) -> list:
    """ Splits a string based on the casing.  Only puts a split when a lowercase letter goes to an uppercase letter.
    This keeps Acronyms together, and ensure Enum words put E with the first word, instead of on its own.
    
    Args:
        string(str) - the string to split
    
    Return:
        the_split_string(list) - a list of 1 element, if no splits occurred, otherwise more elements
    """
    results = []

    # first, we're just going to record where the splits need to happen,
    # then we're going to actually create the splits
    
    split_indices = []
    for i in range(1, len(string)):
        previous_character = string[i-1]
        current_character = string[i]

        # we're only recording indices where a lowercase letter flows into
        # an uppercase letter.  When an uppercase goes to a lowercase, that's
        # just moving from the first letter of a capitalized word, to the second
        # the other way around is moving from one word to a different word,
        # in camel case
        if previous_character.islower() and current_character.isupper():
            split_indices.append(i)

    if not split_indices:
        # there is one or fewer words in this string, so return it
        results.append(string)
    else:
        current_entry = []

        # we need to cache this value, so we can use it for the next split
        last_index = 0
        for index in split_indices:
            word = string[last_index:index]
            current_entry.append(word)
            last_index = index
   
        # also record the last word, which starts at this last split index and goes to the end 
        current_entry.append(string[last_index:])

        for entry in current_entry:
            results.append(entry)
    
    return results



def generate_strings_for_type_symbols(type_symbols: list) -> list:
    results = []
    
    for ts in type_symbols:
        string = ts
        # turn this on to just use the base word
        #string = string.split('_const_ptr_t')[0]
        #string = string.split('_ptr_t')[0]
        #string = string.split('_t')[0]
        
        casing_split = split_camel_case_string(string) 
        if casing_split not in results: 
            results.append(casing_split)
    
    return results


def develop_data_types_c_code_from_type_symbols(type_symbols: list, type_symbol_strings: list) -> str:
    c_code = ''
    
    # add the top of the header ----------------------------------------------------------------------------------------
    year = datetime.date.today().year
    _license = 'MIT License'
    file = 'paciFIST' + __file__.split('paciFIST')[-1]
    
    c_code += f'''
// paciFIST studios. {year}. {_license}'

// This file is auto-generated.  Don't edit it.  
// Instead, edit `generateTypesHeader.py`
// found at: {file}

#ifndef PROJECT_DATA_TYPES_H
#define PROJECT_DATA_TYPES_H

// Global Type Strings are used by the reflection system, to print out the name of the type.
'''
   
   
    # global type strings ----------------------------------------------------------------------------------------------
   
    # add a global type string for each type which was discovered.  It's just
    # the name of the type, as a string, to make it easy to print out later
    def _type_as_global_string(_type_symbol_strings: str):
        string_entry = f'static char const GLOBAL_TYPE_STRING_'
        for tss in _type_symbol_strings:
            string_entry += f'_{tss.upper()}'
        string_value = ''.join(_type_symbol_strings)
        return f'{string_entry}[] = "{string_value}";\n'
    
    # add strings as globals
    for tss in type_symbol_strings:
        ts_global_string = _type_as_global_string(tss)
        c_code += ts_global_string
   
   
    # types enum ------------------------------------------------------------------------------------------------------- 
    
    # add enum struct
    c_code += '\n\ntypedef enum EProjectDataTypes_t {'

    def _as_enum_string(value: str):
        return f'\tEPDT__{value},\n'
    
    for ts in type_symbols:
        ts_val = _as_enum_string(ts)
        c_code += ts_val
    
    c_code += '''
} EProjectDataTypes_t;
#endif // PROJECT_DATA_TYPES_H
    '''
    
    return c_code


def run_program(args: argparse.Namespace):
    print('developing project_types file')
    source_files = discover_source_file_paths(args.project_source_directory)
    type_symbols = parse_files_for_type_symbols(source_files)
    type_symbol_strings = generate_strings_for_type_symbols(type_symbols)
    file_content = develop_data_types_c_code_from_type_symbols(type_symbols, type_symbol_strings) 
    print(f'writing symbols: {file_content}')
    with open(args.write_types_to_path, 'w') as outfile:
        outfile.write(file_content)
    print(f'file: "{args.write_types_to_path}" has been rebuilt')
    print('operation complete')
    return EXIT_NO_ERROR


# program framework ----------------------------------------------------------------------------------------------------

def should_early_out(args: argparse.Namespace) -> bool:
    return False


def get_args(test_args=None):
    """Sets up and uses the argparse module
    
    :argument:
        test_args(list) - a list of args to use instead of command line args.  Implemented for unit testing.
    
    :returns:
        args(argparse.Namespace) - object which allows "dot" access to parsed args
    """    
    
    parser = argparse.ArgumentParser()
    
    parser.add_argument(
        '--write_types_to_path',
        help='write the project_types.h file to this path',
        type=str
    )
   
    parser.add_argument(
        '--project_source_directory',
        help='begin scanning from this directory',
        type=str
    )
    
    args = parser.parse_args(test_args) if test_args else parser.parse_args() 

    # set defaults for this project
    if 'project_source_directory' not in args or not args.project_source_directory:
        setattr(args, 'project_source_directory', f'{PACIFIST_ROOT}/src')
        print(f'Setting default "project_source_directory" = "{args.project_source_directory}"')
    if 'write_types_to_path' not in args or not args.write_types_to_path:
        setattr(args, 'write_types_to_path', f'{PACIFIST_ROOT}/src/project_data_types.h')
        print(f'Setting default "write_types_to_path" = "{args.write_types_to_path}"')

    if should_early_out(args):
        return None

    return args


def main():
    args = get_args()
  
    if not args:
        return EXIT_WITH_ERROR
    
    return run_program(args)
    


if __name__ == '__main__':
    ret = main()    
    sys.exit(ret)
    
