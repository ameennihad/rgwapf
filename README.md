# Rioch GW Arabic Print Filter (rgwapf)

A Unix filter in c larguage to support arabic text printing on Ricoh MFPs/Printers with GW controller
## Supported input file encoding
Currently ISO1256, to support other encodings the charactor map table (charmap[]) can be modified.
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
