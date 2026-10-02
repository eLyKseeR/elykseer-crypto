#!/usr/bin/env bash
# Builds the CI image from ci/Dockerfile.ci.
#
#   ci/build-ci-image.sh [tag]        default tag: code.sbclab.net/elykseer/elykseer-crypto-ci:1.0.0
#   PUSH=1 ci/build-ci-image.sh       also push the image
#
# The external dependencies are built at the submodule commits recorded in
# HEAD, so commit submodule bumps (and changes to ext/Makefile or shell.nix)
# before building. After such a change, build and push a new image, then
# update the image tag in .forgejo/workflows/CI-nix.yaml.
set -euo pipefail

cd "$(git rev-parse --show-toplevel)"

TAG=${1:-code.sbclab.net/elykseer/elykseer-crypto-ci:1.0.0}
PLATFORM=${PLATFORM:-linux/amd64,linux/arm64}

# the prebuilt deps only cover what ext/Makefile builds
EXT_REVS=$(git ls-tree HEAD ext/ | awk '$2 == "commit" && $4 != "ext/sizebounded" { printf "%s=%s ", $4, $3 }')
EXT_HASH=$(ci/ext-hash.sh)

echo "building ${TAG} for ${PLATFORM}"
echo "  EXT_REVS=${EXT_REVS}"
echo "  EXT_HASH=${EXT_HASH}"

docker buildx build \
    --platform "${PLATFORM}" \
    -f ci/Dockerfile.ci \
    --build-arg "EXT_REVS=${EXT_REVS}" \
    --build-arg "EXT_HASH=${EXT_HASH}" \
    -t "${TAG}" \
    $([ "${PUSH:-0}" = 1 ] && echo --push || echo --load) \
    .
