# ~/.bashrc: executed by bash(1) for non-login shells.
# see /usr/share/doc/bash/examples/startup-files (in the package bash-doc)
# for examples

# If not running interactively, don't do anything
case $- in
    *i*) ;;
      *) return;;
esac

# don't put duplicate lines or lines starting with space in the history.
# See bash(1) for more options
HISTCONTROL=ignoreboth

# append to the history file, don't overwrite it
shopt -s histappend

# for setting history length see HISTSIZE and HISTFILESIZE in bash(1)
HISTSIZE=1000
HISTFILESIZE=2000

# check the window size after each command and, if necessary,
# update the values of LINES and COLUMNS.
shopt -s checkwinsize

# If set, the pattern "**" used in a pathname expansion context will
# match all files and zero or more directories and subdirectories.
#shopt -s globstar

# make less more friendly for non-text input files, see lesspipe(1)
[ -x /usr/bin/lesspipe ] && eval "$(SHELL=/bin/sh lesspipe)"

# set variable identifying the chroot you work in (used in the prompt below)
if [ -z "${debian_chroot:-}" ] && [ -r /etc/debian_chroot ]; then
    debian_chroot=$(cat /etc/debian_chroot)
fi

# set a fancy prompt (non-color, unless we know we "want" color)
case "$TERM" in
    xterm-color) color_prompt=yes;;
esac


# uncomment for a colored prompt, if the terminal has the capability; turned
# off by default to not distract the user: the focus in a terminal window
# should be on the output of commands, not on the prompt
force_color_prompt=yes

if [ -n "$force_color_prompt" ]; then
    if [ -x /usr/bin/tput ] && tput setaf 1 >&/dev/null; then
	# We have color support; assume it's compliant with Ecma-48
	# (ISO/IEC-6429). (Lack of such support is extremely rare, and such
	# a case would tend to support setf rather than setaf.)
	color_prompt=yes
    else
	color_prompt=
    fi
fi

if [ "$color_prompt" = yes ]; then
    PS1='${debian_chroot:+($debian_chroot)}\[\033[01;32m\]PrieureDeSion \[\033[01;36m\]@ \[\033[01;32m\]idiot-box\[\033[00m\]:\[\033[01;36m\]\w\[\033[00m\]\$ '
# Here's a change

else
    PS1='${debian_chroot:+($debian_chroot)}\u@\h:\w\$ '
fi
unset color_prompt force_color_prompt

# If this is an xterm set the title to user@host:dir
case "$TERM" in
xterm*|rxvt*)
    PS1="\[\e]0;${debian_chroot:+($debian_chroot)}\u@\h: \w\a\]$PS1"
    ;;
*)
    ;;
esac

# enable color support of ls and also add handy aliases
if [ -x /usr/bin/dircolors ]; then
    test -r ~/.dircolors && eval "$(dircolors -b ~/.dircolors)" || eval "$(dircolors -b)"
    alias ls='ls --color=auto'
    #alias dir='dir --color=auto'
    #alias vdir='vdir --color=auto'

    alias grep='grep --color=auto'
    alias fgrep='fgrep --color=auto'
    alias egrep='egrep --color=auto'
fi

# some more ls aliases
alias ll='ls -alF'
alias la='ls -A'
alias l='ls -CF'

# Add an "alert" alias for long running commands.  Use like so:
#   sleep 10; alert
alias alert='notify-send --urgency=low -i "$([ $? = 0 ] && echo terminal || echo error)" "$(history|tail -n1|sed -e '\''s/^\s*[0-9]\+\s*//;s/[;&|]\s*alert$//'\'')"'

# Alias definitions.
# You may want to put all your additions into a separate file like
# ~/.bash_aliases, instead of adding them here directly.
# See /usr/share/doc/bash-doc/examples in the bash-doc package.

if [ -f ~/.bash_aliases ]; then
    . ~/.bash_aliases
fi

# enable programmable completion features (you don't need to enable
# this, if it's already enabled in /etc/bash.bashrc and /etc/profile
# sources /etc/bash.bashrc).
if ! shopt -oq posix; then
  if [ -f /usr/share/bash-completion/bash_completion ]; then
    . /usr/share/bash-completion/bash_completion
  elif [ -f /etc/bash_completion ]; then
    . /etc/bash_completion
  fi
fi

alias alert='notify-send --urgency=low -i "$([ $? = 0 ] && echo terminal || echo error)" "$(history|tail -n1|sed -e '\''s/^\s*[0-9]\+\s*//;s/[;&|]\s*alert$//'\'')"'
alias audioDL='youtube-dl --extract-audio --audio-format mp3 -l'
# alias cmake='/opt/cmake/bin/cmake'
alias cmk='catkin_make'
alias cmu='cd ~/Documents/CMU\ 2017'
alias egrep='egrep --color=auto'
alias fgrep='fgrep --color=auto'
alias fun='cd ~/Documents/Cool\ Stuff/'
alias grep='grep --color=auto'
alias iit='cd ~/Desktop/IIT\ Bombay\ 2015-19/'
alias l='ls -CF'
alias la='ls -A'
alias ll='ls -alF'
alias ls='ls --color=auto'
alias matlab='/usr/local/MATLAB/R2015b/bin/matlab'
alias pc='cd /'
alias pepit='autopep8 --in-place --aggressive --aggressive'
alias proj='cd ~/Desktop/IIT\ Bombay\ 2015-19/Research/'
alias quartus='/home/dhruv-shah/intelFPGA_lite/16.1/quartus/bin/quartus'
alias result='python ~/git/asc/result.py'
alias resume_gen='java -jar ResumeGenerator.jar "Dhruv Ilesh Shah" 150070016 "Electrical Engineering" "UG Second Year" "IIT Bombay" Male "" "26/01/1998" "Graduation;IIT Bombay;IIT Bombay;2019;9.32" "Intermediate/+2;CBSE;City International School, Pune;2015;96.80" "Matriculation;CBSE;Bharati Vidyapeeth, Pune;2013;10.00"
'
alias resume_gen_short='java -jar ResumeGenerator.jar "Dhruv Ilesh Shah" 150070016 "Electrical Engineering" "UG Second Year" "IIT Bombay" Male "" "26/01/1998" "Graduation;IIT Bombay;IIT Bombay;2019;9.32"'
alias scilab='~/scilab-5.5.2/bin/scilab'
alias sem1='cd ~/Desktop/IIT\ Bombay\ 2015-19/Sem\ 1/'
alias sem2='cd ~/Desktop/IIT\ Bombay\ 2015-19/Sem\ 2/'
alias sem3='cd ~/Desktop/IIT\ Bombay\ 2015-19/Sem\ 3/'
alias sem4='cd ~/Desktop/IIT\ Bombay\ 2015-19/Sem\ 4/'
alias sem5='cd ~/Desktop/IIT\ Bombay\ 2015-19/Sem\ 5/'
alias sem6='cd ~/Desktop/IIT\ Bombay\ 2015-19/Sem\ 6/'
alias ssh_mars='ssh dhruv@10.9.128.12'
alias ssh_pclab30='ssh prieuredesion@10.107.32.30'
alias ssh_pclab44='ssh anon@10.107.32.44'
alias ssh_pclab31='ssh -X student@10.107.32.31'
alias ssh_ravan='ssh -X dhruvshah@10.107.1.5'
alias ssh_rudra='ssh dhruvshah@10.107.1.6'
alias ssh_rudrax='ssh -X dhruvshah@10.107.1.6'
alias ssh_sharada='ssh -X dhruvshah@sharada.ee.iitb.ac.in'
alias view_tex='~/view_tex.sh'
alias webcam='vlc v4l2:///dev/video0'
alias oned='cd ~/OneDrive'
alias setclk='sudo service ntp stop && sudo ntpdate ntp.iitb.ac.in'
alias eagle='~/eagle-7.2.0/bin/eagle'
alias ipe='~/ipe.AppImage'
alias jfr18='cd /home/prieuredesion/Documents/CMU\ 2017/Work/Write-Ups/JFR2018'
# source /opt/ros/kinetic/setup.bash

# export GAZEBO_MODEL_PATH=$GAZEBO_MODEL_PATH:$SIM_DIR
# export GAZEBO_PLUGIN_PATH=$GAZEBO_PLUGIN_PATH:$(roscd;pwd)/lib
# export GAZEBO_RESOURCE_PATH=$GAZEBO_PLUGIN_PATH:$SIM_DIR

export MATLAB_JAVA=/usr/lib/jvm/java-8-openjdk-amd64/jre
export ETS_TOOLKIT=wx # For Mayavi
