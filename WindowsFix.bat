:: Configure this specific Git repository to use sparse-checkout mode (so only
:: selected files are checked out)
git config core.sparsecheckout true

:: Copy the configuration of which files to check out to its proper location
COPY sparse-checkout .git\info\sparse-checkout

:: Ask Git to apply the new policy to our working directory
git read-tree --reset -u HEAD

:: Delete the problematic directory that we don't want
RMDIR /S /Q Classification\goldfishWeka\

:: Done
