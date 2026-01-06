class CustomEscMenuConfig
{
    string CONFIG_VERSION;
    string SERVERNAME;
    int DISPLAYPLAYERSTATUS;
    int DISPLAYSERVER;
    string DiscordLink;
    string DonationLink;
    ref array<string> AdminSteamIDs;
    ref array<string> ServerRules;

    void CustomEscMenuConfig()
    {
        DISPLAYPLAYERSTATUS = 1;
        DISPLAYSERVER = 1;
        DiscordLink = "https://discord.gg/nWbaERkAQR";
        DonationLink = "https://www.gtxgaming.co.uk/clientarea/index.php?m=public_pay&hash_id=WY0o0rNgGQzv";
        AdminSteamIDs = new array<string>();
        ServerRules = new array<string>();
    }
}
