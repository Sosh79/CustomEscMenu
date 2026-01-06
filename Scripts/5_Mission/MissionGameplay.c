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

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);

        // Check for Custom Reload Input defined in inputs.xml
        if (GetGame().GetInput().LocalPress("UACustomEscMenuReload", false))
        {
            // Send Reload Request
            GetRPCManager().SendRPC("CustomEscMenuRPC", "ReloadConfigRequest", null, true, null);
        }
    }
    
    // RPC Registration
    void MissionGameplay()
    {
        GetRPCManager().AddRPC("CustomEscMenuRPC", "ShowReloadNotification", this, SingeplayerExecutionType.Client);
    }
    
    void ShowReloadNotification(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client || !GetGame().IsClient()) return;
        
        // Show notification
        NotificationSystem.AddNotificationExtended(3.0, "Config Reloaded", "Configuration has been successfully reloaded and broadcasted!", "set:dayz_gui_v2 image:tutorials");
    }
}
