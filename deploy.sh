FILE="$1"
FILENAME="$2"
FILELABEL="$3"
GITHUB_TOKEN="$4"
USERNAME="obraunsdorf"
REPONAME="playbook-creator"
GIT_HASH=`git rev-parse HEAD`
GIT_TAG_ABBREV=`git describe --tags --abbrev=0`
GIT_TAG_FULL=`git describe --tags`
if [[ "$GIT_TAG_FULL" && "$GIT_TAG_FULL" == "$GIT_TAG_ABBREV" ]]
then
    go get github.com/github-release/github-release
    GOBIN=`pwd` go install github.com/github-release/github-release
    go install github.com/github-release/github-release
    if [[ "$(./github-release info -s ${GITHUB_TOKEN} -u ${USERNAME} -r ${REPONAME} -t ${GIT_TAG_FULL})" != "No existing release for specified tag"  ]]
    then
        echo "Release for tag ${GIT_TAG_FULL} already exists"
    else 
        echo "Release for tag ${GIT_TAG_FULL} does not exist, creating new one..."
        ./github-release release -s ${GITHUB_TOKEN} -u ${USERNAME} -r ${REPONAME} -t ${GIT_TAG_FULL} -c ${GIT_HASH} --draft -d "This is the release of PlaybookCreator ${GIT_TAG_FULL} </br>CHANGELOG:</br>$CHANGELOG"
    fi
    echo "Uploading artifacts..."
    ./github-release upload -s ${GITHUB_TOKEN} -u ${USERNAME} -r ${REPONAME} -t ${GIT_TAG_FULL} -n "$FILENAME" -l "$FILELABEL" -f "$FILE"
else echo "no deployment necessary (untagged commit)"
fi
