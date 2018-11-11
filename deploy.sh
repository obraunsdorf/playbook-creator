GIT_TAG_ABBREV=`git describe --tags --abbrev=0`
GIT_TAG_FULL=`git describe --tags`
if [[ "$GIT_TAG_FULL" && "$GIT_TAG_FULL" == "$GIT_TAG_ABBREV" ]]
then
    go get github.com/aktau/github-release
    GOBIN="/usr/bin" go install github.com/aktau/github-release
    firstTag=$(git tag | sort -r | head -1)
    secondTag=$(git tag | sort -r | head -2 | awk '{split($0, tags, "\n")} END {print tags[1]}')
    echo "Changes between ${secondTag} and ${firstTag}\n"
    CHANGELOG=`git log  --pretty=format:' * %s' ${secondTag}..${firstTag}`
    github-release info -s ${GITHUB_TOKEN} -u ${CIRCLE_PROJECT_USERNAME} -r ${CIRCLE_PROJECT_REPONAME} -t ${GIT_TAG_FULL}
    ERROR=$?
    if [ $ERROR != 0 ]
    then
        echo "Release for tag ${GIT_TAG_FULL} does not exist, creating new one..."
        github-release release -s ${GITHUB_TOKEN} -u ${CIRCLE_PROJECT_USERNAME} -r ${CIRCLE_PROJECT_REPONAME} -t ${GIT_TAG_FULL} -c ${CIRCLE_SHA1} --draft -d "This is the release of PlaybookCreator ${GIT_TAG_FULL} </br>CHANGELOG:</br>$CHANGELOG"
    else echo "Release for tag ${GIT_TAG_FULL} already exists"
    fi
    echo "Editing release, updating description..."
    github-release edit -s ${GITHUB_TOKEN} -u ${CIRCLE_PROJECT_USERNAME} -r ${CIRCLE_PROJECT_REPONAME} -t ${GIT_TAG_FULL} --draft -d "This is the release of PlaybookCreator ${GIT_TAG_FULL} </br>CHANGELOG:</br>$CHANGELOG"
    echo "Uploading artifacts..."
    github-release upload -s ${GITHUB_TOKEN} -u ${CIRCLE_PROJECT_USERNAME} -r ${CIRCLE_PROJECT_REPONAME} -t ${GIT_TAG_FULL} -n "PlaybookCreator" -l "PlaybookCreator for Linux (CircleCI build ${CIRCLE_BUILD_URL})" -f bin/PlaybookCreator
else echo "no deployment necessary (untagged commit)"
fi