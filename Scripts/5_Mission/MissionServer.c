modded class MissionServer
{
    override void OnInit()
    {
        super.OnInit();
        
        // Load the config on server start
        CustomEscMenuConfigManager.Load();
        
        // Register RPCs
        GetRPCManager().AddRPC("CustomEscMenuRPC", "SyncPlayerCountRequest", this, SingeplayerExecutionType.Server);
        GetRPCManager().AddRPC("CustomEscMenuRPC", "GetConfigRequest", this, SingeplayerExecutionType.Server);
        GetRPCManager().AddRPC("CustomEscMenuRPC", "SyncPlayerStatsRequest", this, SingeplayerExecutionType.Server);
        GetRPCManager().AddRPC("CustomEscMenuRPC", "SyncPlayerListRequest", this, SingeplayerExecutionType.Server);
        GetRPCManager().AddRPC("CustomEscMenuRPC", "ReloadConfigRequest", this, SingeplayerExecutionType.Server);
    }

    void SyncPlayerCountRequest(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Server || !GetGame().IsServer()) return;

        array<Man> players = new array<Man>;
        GetGame().GetPlayers(players);
        int count = players.Count();

        GetRPCManager().SendRPC("CustomEscMenuRPC", "SyncPlayerCount", new Param1<int>(count), true, sender);
    }

    void SyncPlayerStatsRequest(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Server || !GetGame().IsServer()) return;

        PlayerBase player = PlayerBase.Cast(sender.GetPlayer());
        if (!player) return;

        array<int> plyIData = new array<int>;
        array<float> plyFData = new array<float>;

        // Int Data
        // Int Data - Sending PERCENTAGES (0-100) to match layout
        
        // Health
        float health = player.GetHealth("", "Health");
        float healthMax = player.GetMaxHealth("", "Health");
        if (healthMax > 0) plyIData.Insert((health / healthMax) * 100);
        else plyIData.Insert(0);

        // Blood
        float blood = player.GetHealth("", "Blood");
        float bloodMax = player.GetMaxHealth("", "Blood");
        if (bloodMax > 0) plyIData.Insert((blood / bloodMax) * 100);
        else plyIData.Insert(0);

        // Water
        float water = player.GetStatWater().Get();
        float waterMax = player.GetStatWater().GetMax();
        if (waterMax > 0) plyIData.Insert((water / waterMax) * 100);
        else plyIData.Insert(0);

        // Energy
        float energy = player.GetStatEnergy().Get();
        float energyMax = player.GetStatEnergy().GetMax();
        if (energyMax > 0) plyIData.Insert((energy / energyMax) * 100);
        else plyIData.Insert(0);

        // Float Data
        plyFData.Insert(player.StatGet(AnalyticsManagerServer.STAT_DISTANCE));
        plyFData.Insert(player.StatGet(AnalyticsManagerServer.STAT_PLAYTIME));
        plyFData.Insert(player.StatGet(AnalyticsManagerServer.STAT_PLAYERS_KILLED));
        plyFData.Insert(player.StatGet(AnalyticsManagerServer.STAT_INFECTED_KILLED));
        plyFData.Insert(player.StatGet(AnalyticsManagerServer.STAT_LONGEST_SURVIVOR_HIT));

        GetRPCManager().SendRPC("CustomEscMenuRPC", "SyncPlayerStats", new Param2<array<int>, array<float>>(plyIData, plyFData), true, sender);
    }

    void SyncPlayerListRequest(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Server || !GetGame().IsServer()) return;

        array<Man> players = new array<Man>;
        GetGame().GetPlayers(players);
        
        array<string> playerNames = new array<string>;
        foreach (Man playerMan : players)
        {
            PlayerBase player = PlayerBase.Cast(playerMan);
            if (player && player.GetIdentity())
            {
                playerNames.Insert(player.GetIdentity().GetName());
            }
        }

        GetRPCManager().SendRPC("CustomEscMenuRPC", "SyncPlayerList", new Param1<array<string>>(playerNames), true, sender);
    }

    void ReloadConfigRequest(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        // 1. Security Check: Server only
        if (type != CallType.Server || !GetGame().IsServer()) return;

        if (!sender) return;

        CustomEscMenuConfig config = CustomEscMenuConfigManager.GetConfig();
        if (!config) return;

        // 2. Admin Check: Verify SteamID
        bool isAdmin = false;
        string senderID = sender.GetPlainId(); // SteamID64
        
        foreach (string adminID : config.AdminSteamIDs)
        {
            if (adminID == senderID)
            {
                isAdmin = true;
                break;
            }
        }

        if (isAdmin)
        {
            // 3. Reload Config from Disk
            CustomEscMenuConfigManager.Reload();
            config = CustomEscMenuConfigManager.GetConfig(); // Get fresh ref
            
            // 4. Broadcast to ALL players
            array<Man> players = new array<Man>;
            GetGame().GetPlayers(players);
            
            foreach (Man playerMan : players)
            {
                PlayerBase player = PlayerBase.Cast(playerMan);
                if (player && player.GetIdentity())
                {
                    GetRPCManager().SendRPC("CustomEscMenuRPC", "GetConfigResponse", new Param1<CustomEscMenuConfig>(config), true, player.GetIdentity());
                }
            }
            
            // 5. Notify Admin of success
            GetRPCManager().SendRPC("CustomEscMenuRPC", "ShowReloadNotification", null, true, sender);
        }
    }

    void GetConfigRequest(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Server || !GetGame().IsServer()) return;

        // Send the current config to the client
        GetRPCManager().SendRPC("CustomEscMenuRPC", "GetConfigResponse", new Param1<CustomEscMenuConfig>(CustomEscMenuConfigManager.GetConfig()), true, sender);
    }
}
