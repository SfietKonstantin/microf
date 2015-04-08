TEMPLATE = aux

OTHER_FILES += app.coffee \
    package.json

system($$PWD/build.sh)
