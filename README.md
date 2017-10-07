# RICOH GW Arabic Print Filter (rgwapf)

A Unix filter in C language to support Arabic text printing on Ricoh MFPs/Printers with GW controller
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
<p>For older GW model from 05S to 09S, symbol set PC-864 cannot be configured as the default. Print jobs need to contain appropriate PCL symbol set commands to specify the symbol set, this can be done from a script file, not yet implemented in this solution. </p>
<p>For GW models older than 05S, symbol set PC-864 is not supported, therefor this solution cannot be used.
<h2>How to configure print queue to use this solution</h2>
This solution is tested on FreeBSD and IBM AIX 5.3, as a first step we need to install Ricoh Unix Filter this step is common for all Unix/Linux distributions
<h3>Installing Ricoh Unix Filter</h3>
<p>Download and install the right Unix Filter for your printer from Ricoh website, this can be found in the driver download page, at the end of the page look for <span style="font-weight:bold">Firmware/Other OS</span>, under that there is a link for the Unix filter called <span style="font-weight:bold">UNIX</span>, click that link and follow the download and installation instructions included in the file.</p>
<p>For IBM AIX, install standard print queue without device options, don't user virtual queue.</p>
<h3>FreeBSD Configuration</h3>
TBA
<h3>IBM AIX 5.3 Configuration</h3>
<p>In addition to the print queue created with Ricoh Unix filter, we need to create a second queue to execute the filter and pass its output to the first queue.</p>
<p>First download the file (rgwapf.c) to your AIX server and compile it, name the output file (rgwapf) and copy it to (/usr/local/bin/myfilters), also download (aix.filter) to same folder</p>
<p>Execute the following commands to change permission and ownership of both files</p>
<ul>
<li>chmod 755 /usr/local/bin/myfilters/rgwapf</li>
<li>chmod 755 /usr/local/bin/myfilters/sp5300.filter</li>
<li>chown bin:printq /usr/local/bin/myfilters/rgwapf</li>
<li>chown bin:printq /usr/local/bin/myfilters/sp5300.filter</li>
</ul>
