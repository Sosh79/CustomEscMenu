modded class MissionGameplay
{
    override void Pause()
    {
        if (IsPaused() || (GetGame().GetUIManager().GetMenu() && GetGame().GetUIManager().GetMenu().GetID() == MENU_INGAME))
        {
            return;
        }

        m_PauseQueued = true;

        if (g_Game.IsClient() && g_Game.GetGameState() != DayZGameState.IN_GAME)
        {
            return;
        }
        
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (player && !player.IsPlayerLoaded() || IsPlayerRespawning())
        {
            return;
        }
        
        CloseAllMenus();
        
        // Force create OUR specific menu class and show it
        UIScriptedMenu menu = GetUIManager().CreateScriptedMenu(MENU_INGAME, GetGame().GetUIManager().GetMenu());
        if (menu)
        {
            GetUIManager().ShowScriptedMenu(menu, null);
        }

        AddActiveInputExcludes({"menu"});
        AddActiveInputRestriction(EInputRestrictors.INVENTORY);

        m_PauseQueued = false;
    }
}
