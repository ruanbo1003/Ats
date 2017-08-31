
#set -x

path=$(cd $(dirname $0);pwd)
echo $path
iso8859="GBK"
assic="ASCII"
target="UTF-8"


function convert_file()
{
	if [ $2 == 1 ]; then
		#echo "convert $iso8859 file[$1] to target[$target]"
		iconv -f $iso8859 $1 -t $target -o $1
	fi
	
	if [ $2 == 2 ]; then
		echo "convert $assic file[$1] to targrt[$target]"
		#echo iconv -f $assic $1 -t $target -o $1
	fi
}


function handle_file()
{
	for file in `ls $1`
	do
		if [[ -d $1"/"$file ]]; then
			handle_file $1"/"$file
		else
			iend=$[${#file}-4]
			file_type=${file:$iend}
			#echo $file_type
			#echo $file
			
			cd $1		
			
			if [ "$file_type" == ".hpp" -o "$file_type" == ".cpp" ]; then
				#echo $file
				file_encoder=`file $file`
				#echo $file_encoder
			
				ret=$(echo $file_encoder | grep "ISO-8859")
				if [[ "$ret" != "" ]]; then
					echo "a iso8859 file"
					convert_file $1"/"$file 1
				fi

				ret2=$(echo $file_encoder | grep "${assic}")
				if [ "$ret2" != "" ]; then
					echo "a $assic file"
					convert_file $1"/"$file 2
				fi
			fi	
		fi
	done
}

handle_file $path



