
#GROUP_RECORD=$(getent group software) 
#IFS=":" read -ra GROUP_FIELDS <<< "$GROUP_RECORD"
IFS=":" read -ra GROUP_FIELDS <<< "$(getent group software)"
export GROUP_MEMBERS=${GROUP_FIELDS[3]}
echo "Got group members $GROUP_MEMBERS" 
