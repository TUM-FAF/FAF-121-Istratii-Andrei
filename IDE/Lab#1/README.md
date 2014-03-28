# IDE Laboratory Work #1

## Subject

Command Line Interface; CLI Editors; Setting Server Environment; Version Control Systems

## Objectives:
  - Understanding and using CLI (basic level)
  - Administration of a remote linux machine using SSH
  - Ability to work remotely (remote code editing)
  - Ability to work with VCS

## Executed Tasks:
  - **Mandatory Tasks:**
    - Connect to a remote server via SSH
    - Initialize a repository on server
    - Create a file in repository folder, write in your name, save it and commit it
  - **Tasks With Points:**
    - Create an ssh server (2 pt)
    - Connect to server using public key (1 pt)
    - Create 2 more branches with at least one unique committed file per branch (1 pt)
    - Set a branch to track a remote origin on which you are able to push (ex. github, bitbucket or a custom server) (1 pt)
    - Reset a branch to previous commit, reset a branch to some specific commit (1 pt)
    - Restore a reset branch back to its previous state (1 pt)
    - Create a VCS alias (1 pt)
    - Create a VCS hook (1 pt)
    - Git cherry-pick, rebase (1 pt)
    - Learn 10+ Emacs commands (1 pt)
    - Install Haskell mode/code-highlighter for Emacs (1 pt)
  - **Additional Tasks:**
    - Init a bare git repository on the server in order to be able to push to it fromand use it as a remote (git server)


## Theoretical Work:



## Practical Work:

### Connect to a remote server via SSH

Working with SSH in a UNIX environment (in my case Ubuntu Linux) is quite simple, because it doesn't require complex installation process as on Windows platform. To connect to a remote server I used a command of the form `username@hostname:port` where username in my case was 'student' and the hostname was the IP address of the remote server. The port could be omitted because the default settings were used.

### Init a Git repo on the server, create a file, write my name in it

In order to setup a working envirinment on the server I introduced the following commands:

* `mkdir ai` - create a folder to work with
* `cd ai`, `git init` - init an empty repo
* `emacs file.txt`, `Andrei Istratii`, `C-x C-s`, `C-x C-c` - create a file, open it in emacs editor, write my name, save, exit
* `git add .`, `git commit -m "initial commit"` - save changes to the repo

### Create a bare Git repo and use it as a remote Git server

``` sh
# In the 'my_repo.git' folder on the server 
git init --bare

# In the 'ai' local folder (which is also on the server but this doesn't matter)
git remote add origin student@serverip:~/my_repo.git
git push -u origin master # push master branch to the remote repo which is actually on the same machine

```


### Create an SSH server and connect to it using public key authentication

``` sh

# On the server
sudo apt-get install openssh-server

# On client
ssh-keygen
cat ~/.ssh/id_rsa.pub | xsel -i # copy the text to clipboard
ssh username@hostname
# enter the password
mkdir .ssh
cd .ssh
xsel -0 > authorized_keys
exit # close connection
ssh username@hostname # this time no password is required

```


### Editor


* add haskell-mode to emacs text editor by adding a line in the package list in `~/.emacs.d/init.el`

``` lisp

(defvar my-packages '(starter-kit
                      starter-kit-lisp
                      starter-kit-bindings
                      starter-kit-ruby
                      clojure-mode
                      clojure-test-mode
                      nrepl
                      haskell-mode
                      )

```

* learn some emacs commands:
  * `C-n` - move to next line
  * `C-p` - move to previous line
  * `C-f` - move forward one character
  * `C-b` - move backward one character
  * `M-f` - move forward one word
  * `M-b` - move backward one word
  * `C-e` - move to the end of the line
  * `C-a` - move to the begining of the line
  * `C-x C-f` - open/create a file
  * `C-x C-s` - save current buffer
  * `C-x b` - select an opened buffer
  * `C-x C-b` - show the list of open buffers
  * `C-z` - suspend emacs
  * `C-x C-c` - exit emacs
  * `C-g` - cancel the process of entering a command

  

