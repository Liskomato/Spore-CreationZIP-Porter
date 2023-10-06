# Creation ZIP Porter
A foundational Spore ModAPI mod to allow loading Spore creations to and from ZIP files in-game, and to export their raw data. 

This mod will also allow you to load ``.package`` and ``.zip`` files from Spore ModAPI Launcher Kit's `mLibs` folder, allowing you to essentially create `.sporemod` files containing ***both*** creations and the mods required by them. 

The mod will also allow you to download creations from both Spore.com and elsewhere in the internet straight from the cheat console, *without need for a Spore online account*. You can also download all of the adventure's required assets via this, allowing easier ability to play and share adventures in general.

## Features
* Five new cheats
    
    * ``importCreations`` - Download creations into the game.
        
        * `importCreations -a` - Select an adventure from the Sporepedia, and download all of the adventure's assets from Spore.com without an account.
        * ``importCreations -id`` - Downloads a creation based on its ID found in Spore.com. If the creation is an adventure, its required assets will be downloaded as well.
        * ``importCreations <url>`` - Downloads the creation PNG file or extracts the ZIP file from the given (direct) URL address. ZIP files will be extracted automatically and their creations imported to the game.
        * ``importCreations`` (no arguments) - Opens Windows Explorer to select a PNG or ZIP file to be added into the game.
    * ``ZIPExport`` - Select creations from the Sporepedia to be placed in a ZIP file. Adventures will have their required assets downloaded and exported as well, downloading them from Spore.com if necessary.
    * ``assetExport`` - Select creations from the Sporepedia to extract their baked assets. Similar to GMDLExport but exports more files.
    * ``adventureExport`` - Used in the Adventure Editor. It will export any and all files used by the adventure in order to be used in SporeModder FX projects. If the adventure uses modded objects, effects or music, those will be extracted as well.
      * ``adventureExport`` (no arguments) - Default setting of the cheat will export the adventure's loaded models and textures, allowing for a faster loading time when loading it through a custom ``.package`` file made with SporeModder FX.
      * ``adventureExport -noBake`` - Setting the ``-noBake`` flag will export the adventure without the models and textures of the creations, allowing for a much smaller filesize but with the caveat of longer load times in-game.
    * ``CheckForZIPs`` - Refreshes the ZIP Manager to check if new files have been added to its known directories.
* Automatic loading of ZIP and PNG files from ``Documents\My Spore Creations\ZIPs`` and ``%programdata%\Spore ModAPI Launcher Kit\mLibs``.
* Ability to load ``.package`` files into the game from ``%programdata%\Spore ModAPI Launcher Kit\mLibs``.

## FAQ
### How does creating modded adventure packs work with this?
**There are currently two ways to make adventure modpacks.**

#### Option 1 - .sporemod file containing ZIP archives
**This option will require your players to install Creation ZIP Porter beforehand!**

You first create a ZIP folder with all of your creations that you want to be added (for adventures keep in mind you have to include ALL of its required creations as well, and sort the adventure as last in the folder alphabetically), and create a .sporemod file containing the ZIP archive and all of the mods you want to add by renaming a ZIP file's extension from ``.zip`` to ``.sporemod``. 

You will also want to create a ModInfo.xml file containing the information that the ModAPI installer can use to know how to place everything. More on that here: http://davoonline.com/sporemodder/rob55rod/ModAPI/Public/xmlModIdentity_v1_0_1_1.html

Once you've created your .sporemod archive containing everything you want and need, try installing it with the ModAPI Easy Installer to see if everything goes good. Then try launching the game to see if everything has loaded as it should've. 

#### Option 2 - .package file
**This option will work for players WITHOUT Creation ZIP Porter installed!**

First, load the adventure you want to export in the adventure editor. Then when it has loaded, press ``CTRL+SHIFT+C`` and type ``adventureExport`` (``-noBake`` flag optional) into the cheat console. Press ``ENTER`` and the adventure's files should be exported to ``C:/Users/(user)/Documents/My Spore Creations/Archive``. The directory has the format of a SporeModder FX project already, but I would suggest you move the files away from the initial folder to their own SporeModder FX project folder elsewhere in the filesystem.

Once you have made your own SporeModder FX project for the exported files, you can pack them into a ``.package`` file which you can then share with other players in order to share creations. This method will also allow you to pack modded assets and localizations within the same project as well, reducing the required amount of files being installed to play modded adventures.

### What does this mod's existence mean in practice?
For over 14 years now, sharing adventures without access to a Spore account has been a struggle for too many players. You need to know the *exact* creations the adventure uses in order to share them correctly AND place the creations to the game *before* dragging the adventure itself, or otherwise the adventure will have that infamous ban mark because not all of the adventure's creations were present in the player's game by the time they added it to the game. 

While things can still go wrong when sharing creations using this mod (for example, ZIP files not working right or the mod failing to fetch all the files from the server), this mod should provide a well enough relief when it comes to sharing Spore creations with others, especially for offline players with no access to the servers.

You can also have modded adventures that could have their own custom localizations (made possible with [Adventure Localizer](https://github.com/Liskomato/Spore-AdventureLocalizer)), effects or music included within, and sharing those should prove no hassle either, provided you follow the instructions slightly above this section in the FAQ, while also exporting the adventure with the ``ZIPExport`` or ``adventureExport`` cheats. 


### Anything else I should know?
If you have a look at the repository's ``ExampleSporemods`` folder, you can download experimental adventure ``.sporemod`` files containing examples of types of adventures made possible with this tool. They could either be modded with unique components, be localized or even just be normal adventures packed into this format. Be sure to have a look at them at some point!

## Dependencies
* [ZipLib](https://bitbucket.org/wbenny/ziplib/)
* [Spore ModAPI SDK v2.5.278 or higher](https://github.com/emd4600/Spore-ModAPI)

## Credits
- Liskomato: Main developer
- Auntie Owl: Concept and developer of Asset Exporter
- Kade: Concept and idea of ZIP importing from `mLibs`
- Emd4600: Developer of Spore ModAPI, helped with coding issues
- Zarklord: Creator of AlternatePackageLocations and UPE 
- Miikka64: Original creator of the adventure "Master of Time - Interlude", used in one of the example `.sporemod` packages.

## Testers
- 0KepOnline
- I_am_THEdragon
- Your Friend the Dragon
