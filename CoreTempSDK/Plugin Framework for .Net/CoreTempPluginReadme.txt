License:
--------
The material contained in this archive is free to use for non-comercial purposes.
For distribution and commercial applications please contact the author of the material:
Arthur_Liberman@hotmail.com
Arthur Liberman
Alcpu

Creating your .Net plugin:
---------------------
For C# and VB.Net:
	1. Download the Core Temp Plugin Template for Visual Studio (Express editions are supported)
		For Visual Studio 2010: install the 'vsix' file for your language. (Cs - C#, Vb - VB.Net)
		For older Visual Studio versions: copy the 'zip' file for your language. (Cs - C#, Vb - VB.Net)
		(to install the template, navigate to 'Documents' --> 'Visual Studio 20xx' --> 'Templates' --> 'ProjectTemplates'
		 and paste the 'zip' file in that folder as is, do not extract it.)
	2. Start Visual Studio
	3. Click 'File' --> 'New Project' --> select 'Visual C#'\'Visual Basic' --> select 'Core Temp Plugin'.
	4. Give your plugin a name and click 'OK'.
	5. Click 'Build' --> 'Rebuild project'.
	6. You will need all the Dll files + the 'Plugin.cfg' file contained in your 'Debug'\'Release' folders.
	*. The templates are preset to target .Net Framework 2.0 - 3.5, you can reference the v4.0 version of 'CoreTempPluginProxy.dll' and change the project settings if you wish to target 4.0.

For other .Net languages:
	1. Create a new project of your preference.
	2. Extract the Core Temp Plugin Libraries.
	3. Add a reference to 'CoreTempPluginProxy.dll'.
	4. Create a new class and implement the 'CoreTemp.Plugin.IPlugin' interface.
	5. Include the 'PluginNetInterface-x86.dll' and 'PluginNetInterface-x64.dll' files from the Core Temp Plugin Libraries into your project, set their following properties:
		Build Action --> Content
		Copy to Output Directory --> Copy always
	6. Build your plugin.
	7. You will need all the Dll files + the 'Plugin.cfg' file contained in your 'Debug'\'Release' folders.

Configuring your .Net plugin:
------------------------
1. You will notice that the plugin project includes a 'Plugin.cfg' file.
   This file must ALWAYS include the following three fields: (please use the same formatting, no spaces)
	AssemblyName=your-plugin-assembly-filename.dll;	
	TypeName=Full.Qualified.Name;
	ClrVersion=v'#version'; (currently CLR versions v2.0.50727 (.Net 2.0 - 3.5) and v4.0.30319 (.Net 4.0) are supported)
   
   Example1: ClrVersion=v2.0.50727;
   Example2: ClrVersion=v4.0.30319;
	
   The 'TypeName' field must include the full name of your class, such as 'MyPluginNamespace.MyPlugin'.
2. The 'Plugin.cfg' file must always be present for the plugin to work. It must be located under the same folder 
   as your assemblies.

Installing your plugin:
-----------------------
1. Under the folder of the 'Core Temp.exe' file, create a folder called 'plugins'
2. Create a folder in 'plugins' for your plugin.