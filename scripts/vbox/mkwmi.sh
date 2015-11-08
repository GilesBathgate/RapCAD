#!/bin/bash
source config

echo "Building unattended.xml file..."

echo "<?xml version='1.0' encoding='utf-8'?>" > $WMI
echo "<unattend xmlns='urn:schemas-microsoft-com:unattend' xmlns:wcm='http://schemas.microsoft.com/WMIConfig/2002/State'>" >> $WMI
echo "<settings pass='windowsPE'>" >> $WMI
echo "<component name='Microsoft-Windows-International-Core-WinPE' processorArchitecture='x86' publicKeyToken='31bf3856ad364e35' language='neutral' versionScope='nonSxS'>" >> $WMI
echo "<InputLocale>en-US</InputLocale>" >> $WMI
echo "<SystemLocale>en-US</SystemLocale>" >> $WMI
echo "<UILanguage>en-US</UILanguage>" >> $WMI
echo "<UserLocale>en-US</UserLocale>" >> $WMI
echo "</component>" >> $WMI
echo >> $WMI
echo "<component name='Microsoft-Windows-Setup' processorArchitecture='x86' publicKeyToken='31bf3856ad364e35' language='neutral' versionScope='nonSxS'>" >> $WMI
echo "<DiskConfiguration>" >> $WMI
echo "<WillShowUI>OnError</WillShowUI>" >> $WMI
echo "<Disk>" >> $WMI
echo "<DiskID>0</DiskID>" >> $WMI
echo "<WillWipeDisk>true</WillWipeDisk>" >> $WMI
echo "<CreatePartitions>" >> $WMI
echo "<CreatePartition>" >> $WMI
echo "<Order>1</Order>" >> $WMI
echo "<Type>Primary</Type>" >> $WMI
echo "<Extend>true</Extend>" >> $WMI
echo "</CreatePartition>" >> $WMI
echo "</CreatePartitions>" >> $WMI
echo "</Disk>" >> $WMI
echo "</DiskConfiguration>" >> $WMI
echo "<UserData>" >> $WMI
echo "<ProductKey>" >> $WMI
echo "<Key>$KEY</Key>" >> $WMI
echo "<WillShowUI>OnError</WillShowUI>" >> $WMI
echo "</ProductKey>" >> $WMI
echo "<AcceptEula>true</AcceptEula>" >> $WMI
echo "</UserData>" >> $WMI
echo "<ImageInstall>" >> $WMI
echo "<OSImage>" >> $WMI
echo "<InstallTo>" >> $WMI
echo "<DiskID>0</DiskID>" >> $WMI
echo "<PartitionID>1</PartitionID>" >> $WMI
echo "</InstallTo>" >> $WMI
echo "<WillShowUI>OnError</WillShowUI>" >> $WMI
echo "<InstallToAvailablePartition>false</InstallToAvailablePartition>" >> $WMI
echo "</OSImage>" >> $WMI
echo "</ImageInstall>" >> $WMI
echo >> $WMI
echo "<ComplianceCheck>" >> $WMI
echo "<DisplayReport>OnError</DisplayReport>" >> $WMI
echo "</ComplianceCheck>" >> $WMI
echo "</component>" >> $WMI
echo "</settings>" >> $WMI
echo "<settings pass='offlineServicing'>" >> $WMI
echo "<component name='Microsoft-Windows-LUA-Settings' processorArchitecture='x86' publicKeyToken='31bf3856ad364e35' language='neutral' versionScope='nonSxS'>" >> $WMI
echo "<EnableLUA>false</EnableLUA>" >> $WMI
echo "</component>" >> $WMI
echo "</settings>" >> $WMI
echo "<settings pass='oobeSystem'>" >> $WMI
echo "<component name='Microsoft-Windows-Shell-Setup' processorArchitecture='x86' publicKeyToken='31bf3856ad364e35' language='neutral' versionScope='nonSxS'>" >> $WMI
echo "<AutoLogon>" >> $WMI
echo "<Password>" >> $WMI
echo "<Value>$PASS</Value>" >> $WMI
echo "<PlainText>true</PlainText>" >> $WMI
echo "</Password>" >> $WMI
echo "<Enabled>true</Enabled>" >> $WMI
echo "<Username>$USER</Username>" >> $WMI
echo "</AutoLogon>" >> $WMI
echo "<UserAccounts>" >> $WMI
echo "<LocalAccounts>" >> $WMI
echo "<LocalAccount wcm:action='add'>" >> $WMI
echo "<Name>$USER</Name>" >> $WMI
echo "<Group>administrators;users</Group>" >> $WMI
echo "<Password>" >> $WMI
echo "<Value>$PASS</Value>" >> $WMI
echo "<PlainText>true</PlainText>" >> $WMI
echo "</Password>" >> $WMI
echo "</LocalAccount>" >> $WMI
echo "</LocalAccounts>" >> $WMI
echo "</UserAccounts>" >> $WMI
echo "<VisualEffects>" >> $WMI
echo "<FontSmoothing>ClearType</FontSmoothing>" >> $WMI
echo "</VisualEffects>" >> $WMI
echo "<OOBE>" >> $WMI
echo "<ProtectYourPC>3</ProtectYourPC>" >> $WMI
echo "<HideEULAPage>true</HideEULAPage>" >> $WMI
echo "<SkipUserOOBE>true</SkipUserOOBE>" >> $WMI
echo "<SkipMachineOOBE>true</SkipMachineOOBE>" >> $WMI
echo "<NetworkLocation>Home</NetworkLocation>" >> $WMI
echo "</OOBE>" >> $WMI
echo "<FirstLogonCommands>" >> $WMI
echo "<SynchronousCommand wcm:action='add'>" >> $WMI
echo "<Order>1</Order>" >> $WMI
echo "<Description>Turn Off Network Selection pop-up</Description>" >> $WMI
echo "<CommandLine>cmd /c reg add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Network\\NewNetworkWindowOff\"</CommandLine>" >> $WMI
echo "</SynchronousCommand>" >> $WMI
echo "<SynchronousCommand wcm:action='add'>" >> $WMI
echo "<Order>2</Order>" >> $WMI
echo "<Description>Turn Off Driver Signing</Description>" >> $WMI
echo "<CommandLine>cmd /c certutil -f -addstore \"TrustedPublisher\" A:\\$CER</CommandLine>" >> $WMI
echo "</SynchronousCommand>" >> $WMI
echo "<SynchronousCommand wcm:action='add'>" >> $WMI
echo "<Order>3</Order>" >> $WMI
echo "<Description>Install VirtualBox Guest Additions</Description>" >> $WMI
echo "<CommandLine>E:\\VBoxWindowsAdditions.exe /S /depth=32 /xres=1024 /yres=768 /with_autologon</CommandLine>" >> $WMI
echo "</SynchronousCommand>" >> $WMI
echo "<SynchronousCommand wcm:action='add'>" >> $WMI
echo "<Order>4</Order>" >> $WMI
echo "<Description>Turn off Auto Logon</Description>" >> $WMI
echo "<CommandLine>cmd /c reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\" /v AutoAdminLogon /d 0 /f</CommandLine>" >> $WMI
echo "</SynchronousCommand>" >> $WMI
echo "<SynchronousCommand wcm:action='add'>" >> $WMI
echo "<Order>5</Order>" >> $WMI
echo "<Description>Reboot</Description>" >> $WMI
echo "<CommandLine>cmd /c shutdown -f -s -t 00</CommandLine>" >> $WMI
echo "</SynchronousCommand>" >> $WMI
echo "</FirstLogonCommands>" >> $WMI
echo "<TimeZone>GMT Standard Time</TimeZone>" >> $WMI
echo "</component>" >> $WMI
echo "</settings>" >> $WMI
echo "</unattend>" >> $WMI
