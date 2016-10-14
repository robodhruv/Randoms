The file extract.py can be used to extract data values for any given parameter.
Note that the CSV files given to me had some issue (6 values in a certain place) and hence I suggest you copy-paste a set of the original data into another CSV file on a spreadsheet. Works flawless then.


To use, run the file as below:
python <csv-file> <parameter-name>

Examples:
python 20150425_mod.csv GHI_Wh_a
python 20150425_mod.csv Grid\ Current\ Line1_MECH-INV4

Note that entries with a space in between must be passed as '\ ', the escape sequence for a space. So 'Grid Current Line1_MECH-INV4
' would be written as 'Grid\ Current\ Line1_MECH-INV4'.