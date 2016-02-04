# From https://gist.github.com/domenic/ec8b0fc8ab45f39403dd

git init

# inside this git repo we'll pretend to be a new user
git config user.name "Lukas Woodtli (Travis CI)"
git config user.email "woodtli.lukas@gmail.com"

# The first and only commit to this new Git repo contains all the
# files present with the commit message "Deploy to GitHub Pages".
git add -f ./FreeRTOS/doxy
git commit -m "Deploy to GitHub Pages"

# Force push from the current repo's master branch to the remote
# repo's gh-pages branch. (All previous history on the gh-pages branch
# will be lost, since we are overwriting it.) We redirect any output to
# /dev/null to hide any sensitive credential data that might otherwise be exposed.
git push --force --quiet "https://${GH_TOKEN}@github.com/LukasWoodtli/QFreeRTOS" master:gh-pages > /dev/null 2>&1
