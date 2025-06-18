# paciFIST studios. 2025. MIT License

import argparse
import datetime
import os
from pathlib import Path
import re
import sys

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
    regex_match_pattern = 'typedef * (.*_t)'
    regex_matcher = re.compile(regex_match_pattern)

    for file in file_paths:
        if not os.path.isfile(file):
            continue
            
        file_string = read_file_as_string(file)
        matches = regex_matcher.findall(file_string)
        for match in matches:
            pruned = match.split('struct')[-1].strip()
            pruned = pruned.split('enum')[-1].strip()
            pruned = pruned.split('size_t')[-1].strip()
            if pruned not in type_symbol_results:
                type_symbol_results.append(pruned)
          
    return type_symbol_results


def develop_enum_c_code_from_type_symbols(type_symbols: list) -> str:
    def _as_enum_string(value: str):
        return f'\tEProjectType__{value},\n'
   
    year = datetime.date.today().year
    _license = 'MIT License'
    
    enum_c_code = f'// paciFIST studios. {year}. {_license}'
    enum_c_code += '''
// This file is auto-generated.  Don't edit it.  
// Instead, edit `generateTypesHeader.py`

#ifndef PROJECT_TYPES_H
#define PROJECT_TYPES_H

typedef enum EProjectTypes_t {
'''
    
    for ts in type_symbols:
        ts_val = _as_enum_string(ts)
        enum_c_code += ts_val
    
    enum_c_code += '''
} EProjectTypes_t;
#endif // PROJECT_TYPES_H
    '''
    return enum_c_code


def run_program(args: argparse.Namespace):
    print('developing project_types file')
    source_files = discover_source_file_paths(args.project_source_directory)
    type_symbols = parse_files_for_type_symbols(source_files)
    file_content = develop_enum_c_code_from_type_symbols(type_symbols) 
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
        setattr(args, 'write_types_to_path', f'{PACIFIST_ROOT}/src/project_types.h')
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
    
