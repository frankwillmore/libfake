#export LD_PRELOAD=`pwd`/libfake.so.1
export VERBOSE_LIBFAKE=yes
IFS=":" read -ra GROUP_FIELDS <<< "$(getent group software)"
export GROUP_MEMBERS=${GROUP_FIELDS[3]}
#echo "Got group members $GROUP_MEMBERS" 
export GROUP_NAME=$(id -gn)
