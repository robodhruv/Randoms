
clear
declare -a message=("H" "A" "P" "P" "Y" " " "B" "I" "R" "T" "H" "D" "A" "Y" " " "H" "A" "G" "U" "L" "U" " " "B" "A" "B" "L" "U" " " ":')")
echo -n "                                             "
for i in "${message[@]}"
do
 sleep 0.15s
 echo -n "$i"
done
echo " " 
