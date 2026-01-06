class CustomEscMenuConfigManager
{
    private static const string CURRENT_VERSION = "1.0";
    private static ref CustomEscMenuConfig m_Config;

    static CustomEscMenuConfig GetConfig()
    {
        if (!m_Config)
        {
            Load();
        }
        return m_Config;
    }

    static void SetConfig(CustomEscMenuConfig config)
    {
        m_Config = config;
    }

    static void Reload()
    {
        if (FileExist(CEM_CONFIG_FILE))
        {
            JsonFileLoader<CustomEscMenuConfig>.JsonLoadFile(CEM_CONFIG_FILE, m_Config);
        }
    }

    static void Load()
    {
        m_Config = new CustomEscMenuConfig();

        if (GetGame().IsServer())
        {
            if (!FileExist(CEM_CONFIG_ROOT))
            {
                MakeDirectory(CEM_CONFIG_ROOT);
            }

            if (FileExist(CEM_CONFIG_FILE))
            {
                JsonFileLoader<CustomEscMenuConfig>.JsonLoadFile(CEM_CONFIG_FILE, m_Config);
                
                if (m_Config.CONFIG_VERSION != CURRENT_VERSION)
                {
                    m_Config.CONFIG_VERSION = CURRENT_VERSION;
                    Save();
                }
            }
            else
            {
                // Default content
                m_Config.CONFIG_VERSION = CURRENT_VERSION;
                m_Config.SERVERNAME = "Welcome on ZoneX server !! - Hosted By Rebal";
                m_Config.DiscordLink = "https://discord.gg/nWbaERkAQR";
                m_Config.DonationLink = "https://www.gtxgaming.co.uk/clientarea/index.php?m=public_pay&hash_id=WY0o0rNgGQzv";
                m_Config.AdminSteamIDs.Insert("STEAMID64_HERE");
                m_Config.ServerRules.Insert("<h1> ZoneX - General Server Rules</h1>");
                m_Config.ServerRules.Insert("<p>Welcome to <b>ZoneX Server!</b></p>");
                m_Config.ServerRules.Insert("<p>In this harsh world, survival means fighting <b>against nature and other players</b>.</p>");
                m_Config.ServerRules.Insert("<p>There are specific <b>PVP Zones</b> for combat — please respect them and follow the rules below </p>");
                m_Config.ServerRules.Insert("<h2> 1. Respect & Conduct</h2>");
                m_Config.ServerRules.Insert("<p>1. Do not use offensive, racist, or provocative language toward any player or staff member.</p>");
                m_Config.ServerRules.Insert("<p>2. Mutual respect is mandatory — any toxic behavior will be punished immediately.</p>");
                m_Config.ServerRules.Insert("<p>3. Impersonating another player or admin is strictly prohibited.</p>");
                m_Config.ServerRules.Insert("<h2> 2. Cheating & Exploits</h2>");
                m_Config.ServerRules.Insert("<p>1. Using any kind of <b>hack, script, or modification</b> that gives unfair advantage is forbidden.</p>");
                m_Config.ServerRules.Insert("<p>2. Exploiting <b>glitches, bugs, or map issues</b> is not allowed.</p>");
                m_Config.ServerRules.Insert("<p>3. Using any <b>third-party programs</b> that affect gameplay or visibility is not permitted.</p>");
                m_Config.ServerRules.Insert("<h2> 3. Base Building Rules</h2>");
                m_Config.ServerRules.Insert("<p>1. Building inside <b>military or high-loot zones</b> is not allowed.</p>");
                m_Config.ServerRules.Insert("<p>2. Do not block <b>main roads or public pathways</b> with your base.</p>");
                m_Config.ServerRules.Insert("<p>3. Every base must have at least <b>one main accessible entrance</b>.</p>");
                m_Config.ServerRules.Insert("<p>4. Storing vehicles or items in <b>unrealistic spots</b> (rocks, trees, etc.) is forbidden.</p>");
                m_Config.ServerRules.Insert("<h2> 4. PVP Rules</h2>");
                m_Config.ServerRules.Insert("<p>1. Combat is only allowed inside <b>designated PVP Zones</b>.</p>");
                m_Config.ServerRules.Insert("<p>2. Random killing (<b>KOS</b>) outside of PVP zones is not allowed.</p>");
                m_Config.ServerRules.Insert("<p>3. <b>Spawn camping</b> or waiting at respawn areas is strictly prohibited.</p>");
                m_Config.ServerRules.Insert("<p>4. Killing players inside <b>Safe Zones or Trade Areas</b> is not allowed.</p>");
                m_Config.ServerRules.Insert("<p>5. If a player logs out while inside a <b>PVP Zone</b>, they will be automatically moved to a <b>safe area</b></p>");
                m_Config.ServerRules.Insert("<p>outside the zone to prevent exploitation.</p>");
                m_Config.ServerRules.Insert("<h2> 5. Vehicle Rules</h2>");
                m_Config.ServerRules.Insert("<p>1. Vehicle theft is not allowed inside <b>Safe Zones</b>.</p>");
                m_Config.ServerRules.Insert("<p>2. Vehicles left in public areas are considered unprotected and can be stolen.</p>");
                m_Config.ServerRules.Insert("<p>3. Do not destroy vehicles without a valid reason — griefing is not tolerated.</p>");
                m_Config.ServerRules.Insert("<h2> 6. Safe Zones / Main Trade Area</h2>");
                m_Config.ServerRules.Insert("<p>1. No combat or stealing is allowed inside <b>Safe Zones</b>.</p>");
                m_Config.ServerRules.Insert("<p>2. Drive carefully and respect other players inside Safe Zones.</p>");
                m_Config.ServerRules.Insert("<p>3. Do not drop <b>trash or items</b> inside the <b>Main Trade Area</b> — keep it clean.</p>");
                m_Config.ServerRules.Insert("<p>4. Do not leave your vehicle inside the <b>Main Trade Area</b> when you log off.</p>");
                m_Config.ServerRules.Insert("<p>5. Before leaving the server, park your vehicle inside the <b>Garage</b>.</p>");
                m_Config.ServerRules.Insert("<p>6. Breaking Safe Zone rules may result in <b>temporary or permanent bans</b>.</p>");
                m_Config.ServerRules.Insert("<h2> 7. Admin & Staff Rules</h2>");
                m_Config.ServerRules.Insert("<p>1. All <b>admin decisions</b> are final — do not argue or disrespect staff.</p>");
                m_Config.ServerRules.Insert("<p>2. You may submit a <b>ticket or report</b> through Discord for complaints.</p>");
                m_Config.ServerRules.Insert("<p>3. Lying to staff or making false reports will result in punishment.</p>");
                m_Config.ServerRules.Insert("<h2> 8. Updates & Changes</h2>");
                m_Config.ServerRules.Insert("<p>1. Rules may be <b>updated or modified</b> anytime without notice.</p>");
                m_Config.ServerRules.Insert("<p>2. Players must check the rules channel regularly to stay updated.</p>");
                Save();
            }
        }
    }

    static void Save()
    {
        if (GetGame().IsServer())
        {
            if (m_Config)
            {
                // Ensure directory exists before saving (defensive)
                if (!FileExist(CEM_CONFIG_ROOT))
                {
                    MakeDirectory(CEM_CONFIG_ROOT);
                }

                m_Config.CONFIG_VERSION = CURRENT_VERSION;
                JsonFileLoader<CustomEscMenuConfig>.JsonSaveFile(CEM_CONFIG_FILE, m_Config);
            }
        }
    }
}
