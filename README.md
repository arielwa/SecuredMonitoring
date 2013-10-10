SecuredMonitoring
=================

Note for using this repository in Windows
-----------------------------------------

The Android kernel is incompatible with Windows (it has multiple filenames with the same name, but different letter casing, which are considered different files in Linux but not in Windows), so after you clone this repository, please run the WindowsFix.bat script to fix it so that you won't have the goldfishWeka kernel in your local repository.

Concise Instructions
--------------------

1.  Open Command Prompt (WinKey+R, then type in "cmd") and navigate to the
    directory in which you want to clone the repository.
    For example, if you want the project to be in
    'C:\Users\AceVentura\Git\SecuredMonitoring', then go to
    'C:\Users\AceVentura\Git':
    
    CD Your_Chosen_Directory

2.  Clone the repository:

    git clone https://github.com/arielwa/SecuredMonitoring.git SecuredMonitoring
    
3.  Run the script:
    
    cd SecuredMonitoring
    WindowsFix.bat
    
4.  Check the status of your acquired repository:
    
    git status
    
5.  You should see the following message, and nothing else (if you do see
    something else, try to figure out what's wrong and fix it yourself;
    if can't get it to work, ask Tomer (tomergod@post.bgu.ac.il) to give
    you a hand):
    
    # On branch master
    nothing to commit, working directory clean
    
6.  You can now begin working. Have fun! =)
