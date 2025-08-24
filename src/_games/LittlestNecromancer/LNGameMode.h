// paciFIST studios. 2025. MIT License

#ifndef LITTLEST_NECROMANCER_GAME_MODE_H 
#define LITTLEST_NECROMANCER_GAME_MODE_H 

typedef enum ELNGameMode_t{
    ELNGM_UNINITIALIZED = 0,

    /* In this state, the application goes though its boot sequence and opening credits */
    ELNGM_APPLICATION_STARTUP,
    /* In this state, the application is capable of continuing a game, starting a new game, et al */
    ELNGM_APPLICATION_MAIN_MENU,
    /* If left idle on the main menu for too long, this demo state begins and plays a looping trailer */
    ELNGM_APPLICATION_DEMO,
    /* In this state, the application will perform loading, and show a loading screen */
    ELNGM_APPLICATION_LOADING,
    /* In this state, the application allows the user to access various settings about the game */
    ELNGM_APPLICATION_SETTINGS,
    /* In this state, the application allows the user to interact with the about screen */
    ELNGM_APPLICATION_ABOUT,
    /* In this state, gameplay is occurring, and the PlayerCharacter can perform various gameplay interactions */
    ELNGM_GAMEPLAY,
    /* In this state, gameplay is occurring, but is paused */
    ELNGM_GAMEPLAY_PAUSE,
    /* In this state, gameplay is occurring, but the PlayerCharacter is in Character Creator Mode */
    ELNGM_GAMEPLAY_CHARACTER_CREATOR,
    /* In this state, gameplay is occurring, but decorator mode is active */
    ELNGM_GAMEPLAY_DECORATE_AREA,
    /* In this state, gameplay is occurring, but a dialogue is playing. */
    ELNGM_GAMEPLAY_DIALOGUE,
    /* In this state, gameplay is occurring, but an inventory screen is active */
    ELNGM_GAMEPLAY_INVENTORY,
    /* In this state, gameplay is occurring, but an NPC friendship screen is active */
    ELNGM_GAMEPLAY_NPC_FRIENDS,
    /* In this state, gameplay is occurring, but a quest menu is active */
    ELNGM_GAMEPLAY_QUEST_TRACKER,
    /* In this state, gameplay is occurring, but the PlayerCharacter is performing some kind of trade */
    ELNGM_GAMEPLAY_TRADE,
    /* In this state, gameplay is occurring, and the PlayerCharacter is navigating the world map */
    ELNGM_GAMEPLAY_WORLD_MAP_NAVIGATION,


    /* this should always come last */ 
    ELNGM_COUNT

} ELNGameMode_t;
#endif  //LITTLEST_NECROMANCER_GAME_MODE_H