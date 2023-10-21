@echo off

:: *************************** !! IMPORTANT !! ***************************
::
:: Required vars to set: EnginePath, ProjectPureName, VersionSelector
::
:: *************************** !! IMPORTANT !! ***************************

rem Unreal Engine params
::set EnginePath_4=C:\Epic Games\UE Binary\UE_4.27
set EnginePath=c:\Program Files\Epic Games\UE_5.3
set VersionSelector=C:\Program Files (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe
rem Editor path (Note: path for UE4: "..\UE4Editor.exe")
set EditorPath=%EnginePath%\Engine\Binaries\Win64\UnrealEditor.exe
set RunUATPath=%EnginePath%\Engine\Build\BatchFiles\RunUAT.bat

rem Project params
set ProjectRoot=%~dp0..
:: Pure name of your project. Example: name of you project file is TPS.uproject, then ProjectPureName=TPS
set ProjectPureName=SnakeGame
set ProjectName=%ProjectPureName%.uproject
set ProjectPath=%ProjectRoot%\%ProjectName%
set SourceCodePath=%ProjectRoot%\Source

rem Build params
set Platform=Win64
:: Shipping, DebugGame
set Configuration=Development
set ArchivePath=%ProjectRoot%\Build

rem Other params
set dirsToRemove=Intermediate DerivedDataCache Saved Binaries .vs Build Documentation
set filesToRemove=*.sln

rem Target generation params
set COPYRIGHT_LINE=// Snake Game, Copyright LifeEXE. All Rights Reserved.
set EXTRA_MODULE_NAMES="%ProjectPureName%"
set TargetTemplateFilePath=%ProjectRoot%\devops_ue\targets\GameModule.Target.cs.template

rem Run
set ServerExePath=%ProjectRoot%\Build\WindowsServer\%ProjectPureName%Server.exe
set ClientExePath=%ProjectRoot%\Build\WindowsClient\%ProjectPureName%Client.exe
set GameExePath=%ProjectRoot%\Build\Windows\%ProjectPureName%.exe

rem Tests
set TestNames=Snake
set TestOutputLogPath=%ProjectRoot%\Build\Tests\Tests.log
set ReportOutputPath=%ProjectRoot%\Build\Tests
set ExludedPathForTestReport=%SourceCodePath%\%ProjectPureName%\Tests
set UEAutomationContentPath=%EnginePath%\Engine\Content\Automation
set OpenCPPCoveragePath=C:\Program Files\OpenCppCoverage\OpenCppCoverage.exe
