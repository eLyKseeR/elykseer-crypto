#!/usr/bin/env bash
# Builds the CI image from ci/Dockerfile.ci.
#
#   ci/build-ci-image.sh [version]    default: 1.0.0 -> code.sbclab.net/elykseer/elykseer-crypto-ci:1.0.0
#   ARCHS="amd64" ci/build-ci-image.sh   only build these architectures (default: amd64 arm64)
#   PUSH=0 ci/build-ci-image.sh       only build and load locally, do not push
#
# Builds one image per architecture (<tag>-amd64, <tag>-arm64) with Docker
# (non-OCI) media types and no attestations, pushes them, then combines them
# into a manifest list under <tag>.
#
# The external dependencies are built at the submodule commits recorded in
# HEAD, so commit submodule bumps (and changes to ext/Makefile or shell.nix)
# before building. After such a change, build and push a new image, then
# update the image tag in .forgejo/workflows/CI-nix.yaml.
set -euo pipefail

cd "$(git rev-parse --show-toplevel)"

TAGBASE="code.sbclab.net/elykseer/elykseer-crypto-ci"
VTAG=${1:-1.0.0}
ARCHS=${ARCHS:-amd64 arm64}
PUSH=${PUSH:-1}

TAG="${TAGBASE}:${VTAG}"

# the prebuilt deps only cover what ext/Makefile builds
EXT_REVS=$(git ls-tree HEAD ext/ | awk '$2 == "commit" && $4 != "ext/sizebounded" { printf "%s=%s ", $4, $3 }')
EXT_HASH=$(ci/ext-hash.sh)

echo "building ${TAG} for ${ARCHS}"
echo "  EXT_REVS=${EXT_REVS}"
echo "  EXT_HASH=${EXT_HASH}"

ARCH_TAGS=()
for arch in ${ARCHS}; do
    arch_tag="${TAG}-${arch}"
    ARCH_TAGS+=("${arch_tag}")
    echo "building ${arch_tag}"
    if [ "${PUSH}" = 1 ]; then
        output="type=image,name=${arch_tag},push=true,oci-mediatypes=false"
    else
        output="type=docker,name=${arch_tag}"
    fi
    docker buildx build \
        --platform "linux/${arch}" \
        -f ci/Dockerfile.ci \
        --build-arg "EXT_REVS=${EXT_REVS}" \
        --build-arg "EXT_HASH=${EXT_HASH}" \
        --provenance=false --sbom=false \
        --output "${output}" \
        .
done

if [ "${PUSH}" = 1 ]; then
    echo "combining ${ARCH_TAGS[*]} into ${TAG}"
    docker manifest rm "${TAG}" 2>/dev/null || true
    docker manifest create "${TAG}" "${ARCH_TAGS[@]}"
    docker manifest push "${TAG}"
fi
