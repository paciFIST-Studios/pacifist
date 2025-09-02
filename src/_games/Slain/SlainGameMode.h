// paciFIST studios. 2025. MIT License

#ifndef SLAIN_GAME_MODE_H 
#define SLAIN_GAME_MODE_H 

typedef enum ESLGameMode_t{
    ESLGM_UNINITIALIZED = 0,

    /* In this state, the application goes though its boot sequence and opening credits */
    ESLGM_APPLICATION_STARTUP,
    /* In this state, the application is capable of continuing a game, starting a new game, et al */
    ESLGM_APPLICATION_MAIN_MENU,
    /* If left idle on the main menu for too long, this demo state begins and plays a looping trailer */
    ESLGM_APPLICATION_DEMO,
    /* In this state, the application will perform loading, and show a loading screen */
    ESLGM_APPLICATION_LOADING,
    /* In this state, the application allows the user to access various settings about the game */
    ESLGM_APPLICATION_SETTINGS,
    /* In this state, the application allows the user to interact with the about screen */
    ESLGM_APPLICATION_ABOUT,
    /* In this state, gameplay is occurring, and the PlayerCharacter can perform various gameplay interactions */
    ESLGM_GAMEPLAY,
    /* In this state, gameplay is occurring, but is paused */
    ESLGM_GAMEPLAY_PAUSE,
    /* In this state, gameplay is occurring, but the PlayerCharacter is in Character Creator Mode */
    ESLGM_GAMEPLAY_CHARACTER_CREATOR,
    /* In this state, gameplay is occurring, but decorator mode is active */
    ESLGM_GAMEPLAY_DECORATE_AREA,
    /* In this state, gameplay is occurring, but a dialogue is playing. */
    ESLGM_GAMEPLAY_DIALOGUE,
    /* In this state, gameplay is occurring, but an inventory screen is active */
    ESLGM_GAMEPLAY_INVENTORY,
    /* In this state, gameplay is occurring, but an NPC friendship screen is active */
    ESLGM_GAMEPLAY_NPC_FRIENDS,
    /* In this state, gameplay is occurring, but a quest menu is active */
    ESLGM_GAMEPLAY_QUEST_TRACKER,
    /* In this state, gameplay is occurring, but the PlayerCharacter is performing some kind of trade */
    ESLGM_GAMEPLAY_TRADE,
    /* In this state, gameplay is occurring, and the PlayerCharacter is navigating the world map */
    ESLGM_GAMEPLAY_WORLD_MAP_NAVIGATION,


    /* this should always come last */ 
    ESLGM_COUNT

} ESLGameMode_t;
#endif  // SLAIN_GAME_MODE_H