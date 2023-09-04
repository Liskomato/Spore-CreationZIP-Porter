# Creation ZIP Porter
A foundational Spore ModAPI mod to allow loading Spore creations to and from ZIP files in-game. 

This mod will also allow you to load ``.package`` and ``.zip`` files from Spore ModAPI Launcher Kit's `mLibs` folder, allowing you to essentially create `.sporemod` files containing ***both*** creations and the mods required by them. You will have to pack your creation PNGs into a ZIP folder first before adding them to the `.sporemod` package in order to keep things clean, but placing a few PNGs on their own should also be fine.

## FAQ
### How does creating modded adventure packs work with this?
You first create a ZIP folder with all of your creations that you want to be added (for adventures keep in mind you have to include ALL of its required creations as well, and sort the adventure as last in the folder alphabetically), and create a .sporemod file containing the ZIP archive and all of the mods you want to add. 

You will also want to create a ModInfo.xml file containing the information that the ModAPI installer can use to know how to place everything. More on that here: http://davoonline.com/sporemodder/rob55rod/ModAPI/Public/xmlModIdentity_v1_0_1_1.html

Once you've created your .sporemod archive containing everything you want and need, try installing it with the ModAPI Easy Installer to see if everything goes good. Then try launching the game to see if everything has loaded as it should've. 

### What does this mean in practice?
You can have modded adventures that have their own custom, unique music, and perhaps even have them localized so they appear in the player's local language in-game.

Another feature of this mod is the ability to select PNGs (and ZIPs) to be added in-game via Windows Explorer open-with interface (activated with the cheat `DownloadCreation`), drag-and-drop ZIP files into the game window like PNGs, and download creation PNGs/ZIPs directly without server access with the cheat ``ImportFromURL``. The former could be highly useful if you usually play in full screen mode (not borderless), while the second means you just need to download the ZIP file, and boom, you can just drag it immediately to the game without extraction.

### Anything else I should know?
Kade (Spore-Community on GitHub) should also be currently working on a ZIP packer tool that should in-practice allow easier ability to download stuff from Spore.com without relying on the game itself directly. It should synergize with this tool mod well enough, even when taking into consideration the mod's own capabilities.

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
