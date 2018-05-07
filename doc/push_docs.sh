#!/bin/sh

set -e

# Get the current gh-pages branch
git clone -b gh-pages https://git@$GH_REPO_REF
cd $GH_REPO_NAME

git config --global push.default simple
git config user.name "Travis CI"
git config user.email "travis@travis-ci.org"

rm -rf *

cd ..

# Create a clean working directory for this script.
mkdir build_docs
cd build_docs

echo 'Generating Doxygen code documentation...'
cmake ..
make
cd ..

if [ -d "build_docs/html" ] && [ -f "build_docs/html/index.html" ]; then
    cp -r build_docs/html/* $GH_REPO_NAME
    cd $GH_REPO_NAME

    echo 'Uploading documentation to the gh-pages branch...'

    git add --all
    git commit -m "Deploy Goliath docs to GitHub Pages. Build: ${TRAVIS_BUILD_NUMBER}" -m "Commit: ${TRAVIS_COMMIT}"

    git push --force "https://${GH_REPO_TOKEN}@${GH_REPO_REF}" > /dev/null 2>&1
else
    echo 'Warning: No documentation files have been found!' >&2
    exit 1
fi