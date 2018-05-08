#!/bin/sh

set -e # Exit with nonzero exit code if anything fails

SOURCE_BRANCH="master"
TARGET_BRANCH="gh-pages"
OUT=html

# Pull requests and commits to other branches shouldn't try to deploy
if [ "$TRAVIS_PULL_REQUEST" != "false" -o "$TRAVIS_BRANCH" != "$SOURCE_BRANCH" ]; then
    echo "Skipping deploy"
    exit 0
fi

# Clone the existing gh-pages for this repo into $OUT/
git clone -b $TARGET_BRANCH https://git@$GH_REPO_REF $OUT

# Clean out existing contents
rm -rf $OUT/**/* || exit 0

# Configure git
cd $OUT
git config --global push.default simple
git config user.name "Travis CI"
git config user.email "travis@travis-ci.org"

cd ..

# Create a clean working directory for this script.
mkdir build_docs
cd build_docs

echo 'Generating Doxygen documentation...'
cmake -DCMAKE_BUILD_TYPE=Release ..
make
cd ..

if [ -d "build_docs/html" ] && [ -f "build_docs/html/index.html" ]; then
    cp -r build_docs/html/* $OUT
    cd $OUT

    # If there are no changes (e.g. this is a README update) then just bail.
    if [ $(git status --porcelain | wc -l) -lt 1 ]; then
        echo "No changes to the output on this push; exiting."
        exit 0
    fi

    echo 'Uploading documentation to the gh-pages branch...'

    # Commit the "changes", i.e. the new version.
    # The delta will show diffs between new and old versions.
    git add -A .
    git commit -m "Deploy Goliath docs to GitHub Pages. Build: ${TRAVIS_BUILD_NUMBER}" -m "Commit: ${TRAVIS_COMMIT}"

    # Now that we're all set up, we can push.
    git push --force "https://${GH_REPO_TOKEN}@${GH_REPO_REF}" > /dev/null 2>&1
else
    echo 'Warning: No documentation files have been found!' >&2
    exit 1
fi