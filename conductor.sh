#!/bin/bash

choice=$(zenity  --title="Request selection" --list --text="Select a request type" --radiolist --column="Select" --column="Request Type" FALSE "Verify annual pass" FALSE "Verify daily pass" FALSE "Issue daily pass" --height=270 --width=380 --ok-label="Proceed")

case $choice in 

	"Verify annual pass")
						list=$(zenity --title="Annual" --forms --add-entry="FingerPrint Key :" --add-entry="Current Stage :" --add-entry="Route Number :" --add-combo="Select Direction" --combo-values="0|1" --ok-label="verify" --height=350 --width=350)
						if [ $? -ne 0 ]; then exit 1; fi
						set $(echo $list | sed 's/|/\ /g')
						output=$( $PWD/annual_pass $1 $2 $3 $4 2>&1)
						zenity --info --title="Output" --text="$output" --height=70 --width=200
						;;
	"Verify daily pass")
						label=$(zenity --title="Daily" --entry="Enter FingerPrint Key" --ok-label="verify") 
						if [ $? -ne 0 ]; then exit 1; fi
						output=$( $PWD/daily_var $label 2>&1 )
						zenity --info --title="Output" --text="$output" --height=100 --width=200
						;;
	"Issue daily pass") 
						label=$( zenity --title="Daily:Reg" --forms --add-entry="FingerPrint Key :" --add-entry="Mobile Number :" --add-entry="Gender :"  --ok-label="verify" --height=350 --width=350 )
						if [ $? -ne 0 ]; then exit 1; fi
						set $(echo $label | sed 's/|/\ /g' )
						output=$( $PWD/daily_reg $1 $2 $3 2>&1 )
						zenity --info --title="Output" --text="$output" --height=100 --width=200
						;;
esac

exit
