# RICOH GW Arabic Print Filter (rgwapf)

A Unix filter in C language to support Arabic text printing on Ricoh MFPs/Printers with GW controller.
## Supported input file encoding
Currently ISO1256, to support other encodings the character map table (charmap[]) can be modified.
## Supported Printers
Any Ricoh printer with GW controller 09A models or above, the default value of two PCL settings need to be changed as mentioned in the table below, this can be done from User Tools -> Printer Features -> PCL Menu or from Web Image Monitor -> configuration -> Printer -> Basic Settings -> PCL Menu:
  <table class="table table-bordered table-striped">
      <thead>
          <tr>
              <th style="">Setting</th>
              <th style="">Value</th>
          </tr>
      </thead>
      <tbody>
      <tr valign="top"><td>Font Number</td><td valign="top">56</td></tr>
      <tr valign="top"><td>Symbol Set</td><td valign="top">PC-864</td></tr></tbody></table>
For older GW model from 05S to 09S, symbol set PC-864 cannot be configured as the default. Print jobs need to contain appropriate PCL symbol set commands to specify the symbol set, this can be done from a script file, not yet implemented in this solution.
For GW models older than 05S, symbol set PC-864 is not supported, therefor this solution cannot be used.
## How to configure print queue to use this solution
This solution is tested on FreeBSD and IBM AIX 5.3, as a first step we need to install Ricoh Unix Filter this step is common for all Unix/Linux distributions
## Installing Ricoh Unix Filter
Download and install the right Unix Filter for your printer from Ricoh website, this can be found in the driver download page, at the end of the page look for **Firmware/Other OS**, under that there is a link for the Unix filter called **UNIX**, click that link and follow the download and installation instructions included in the file.
For IBM AIX, install standard print queue without device options, don't user virtual queue.
In the next sections I'm assuming that the print queue create here is called (lp_sp5300dn) and the print host name is halled (sp5300dn), you can choose any name you want but make sure you change the name in the script as well.
You need to add printer host name and IP address to your /etc/hosts file.
### FreeBSD Configuration
TBA
### IBM AIX 5.3 Configuration
In addition to the print queue created with Ricoh Unix filter, we need to create a second queue to execute the filter and pass its output to the first queue.
First download the file (rgwapf.c) to your AIX server and compile it, name the output file (rgwapf) and copy it to (/usr/local/bin/myfilters), also download (aix.filter) to same folder, rename the file to match your print queue, in the following steps I'm assuming the file is renamed to sp5300.filter.
Execute the following commands to change permission and ownership of both files.
<ul>
<li>chmod 755 /usr/local/bin/myfilters/rgwapf</li>
<li>chmod 755 /usr/local/bin/myfilters/sp5300.filter</li>
<li>chown bin:printq /usr/local/bin/myfilters/rgwapf</li>
<li>chown bin:printq /usr/local/bin/myfilters/sp5300.filter</li>
</ul>
<p>Edit the file (sp5300.filter) and change the value of (printer) varibale to match the queue name created by Ricoh Unix Filter.</p>
<p>Add a second queue using “smit spooler”, this will be the frontend queue, select “Add a Print Queue” then “other” and set queue parameters as follows:</p>
<ul>
<li>Queue Name: sp5300</li>
<li>Device Name: sp5300.dev</li>
<li>BACKEND PROGAM pathname: /usr/local/bin/myfilters/sp5300.filter</li>
<li>Access: Read/Write (Not sure if this is necessary)</li>
</ul>
<p>At this point your queue configuration file should include the following:</p>
'''
lp_sp5300dn:
	device = @sp5300dn
	up = TRUE
	host = sp5300dn
	s_statfilter = /usr/lib/lpd/aixshort
	l_statfilter = /usr/lib/lpd/aixlong
	rq = text
@sp5300dn:
	backend = /usr/lib/lpd/rembak
sp5300:
	device = sp5300.dev
	up = TRUE
sp5300.dev:
	access = both
	backend = /usr/local/bin/myfilters/sp5300.filter
'''  
Test printing using this command (lp -d sp5300 file_name).
