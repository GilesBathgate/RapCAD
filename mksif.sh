#!/bin/bash
source config

echo "Building sif file..."

echo "[Data]" > $SIF
echo "AutoPartition = 1" >> $SIF
echo "MsDosInitiated = 0" >> $SIF
echo "UnattendedInstall = Yes" >> $SIF
echo "AutomaticUpdates = No" >> $SIF
echo >> $SIF
echo "[Unattended]" >> $SIF
echo "UnattendMode = FullUnattended" >> $SIF
echo "OemSkipEula = Yes" >> $SIF
echo "OemPreinstall = No" >> $SIF
echo "TargetPath = \WINDOWS" >> $SIF
echo "Repartition = Yes" >> $SIF
echo "UnattendSwitch = Yes" >> $SIF
echo "DriverSigningPolicy = Ignore" >> $SIF
echo "WaitForReboot = No" >> $SIF
echo >> $SIF
echo "[GuiUnattended]" >> $SIF
echo "AdminPassword = \"$PASS\"" >> $SIF
echo "AutoLogon = Yes" >> $SIF
echo "AutoLogonCount = 1" >> $SIF
echo "OEMSkipRegional = 1" >> $SIF
echo "OemSkipWelcome = 1" >> $SIF
echo "TimeZone = 85" >> $SIF
echo "ServerWelcome = No" >> $SIF
echo >> $SIF
echo "[UserData]" >> $SIF
echo "ProductKey = \"$KEY\"" >> $SIF
echo "FullName = \"user\"" >> $SIF
echo "OrgName = \"\"" >> $SIF
echo "ComputerName = \"computer\"" >> $SIF
echo >> $SIF
echo "[Identification]" >> $SIF
echo "JoinWorkgroup = WORKGROUP" >> $SIF
echo >> $SIF
echo "[Networking]" >> $SIF
echo "InstallDefaultComponents = Yes" >> $SIF
echo >> $SIF
echo "[Display]" >> $SIF
echo "XResolution = 1024" >> $SIF
echo "YResolution = 768" >> $SIF
echo "BitsPerPel = 32" >> $SIF
echo "AutoConfirm = 1" >> $SIF
echo >> $SIF
echo "[GuiRunOnce]" >> $SIF
echo "Command0=\"C:\WINDOWS\System32\cmd.exe /c start /wait E:\VBoxWindowsAdditions.exe /S /depth=32 /xres=1024 /yres=768 /with_autologon && shutdown -f -s -t 00\"" >> $SIF
