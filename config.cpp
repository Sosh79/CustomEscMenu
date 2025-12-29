class CfgPatches {
  class CustomEscMenu {
    units[] = {};
    weapons[] = {};
    requiredVersion = 0.1;
    requiredAddons[] = {"DZ_Data", "DZ_Scripts"};
  };
};

class CfgMods {
  class CustomEscMenu {
    dir = "CustomEscMenu";
    picture = "";
    action = "";
    hideName = 1;
    hidePicture = 1;
    name = "Custom Escape Menu";
    credits = "SOSH79";
    author = "SOSH79";
    authorID = "0";
    version = "1.0";
    extra = 0;
    type = "mod";
    dependencies[] = {"Mission"};
    class defs {
      class missionScriptModule {
        value = "";
        files[] = {"CustomEscMenu/Scripts/5_Mission"};
      };
    };
  };
};
