export VERBOSE_LIBFAKE=yes
export GROUP_NAME=$(id -gn)
#export GROUP_NAME=software
#IFS=":" read -ra GROUP_FIELDS <<< "$(getent group software)"
IFS=":" read -ra GROUP_FIELDS <<< "$(getent group ${GROUP_NAME})"
export GROUP_MEMBERS=${GROUP_FIELDS[3]}
