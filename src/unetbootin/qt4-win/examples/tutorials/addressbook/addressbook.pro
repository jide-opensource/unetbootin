TEMPLATE  = subdirs
SUBDIRS   = part1 part2 part3 part4 part5 part6 part7

# install
target.path = $$[QT_INSTALL_EXAMPLES]/tutorials/addressbook
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS tutorial.pro README
sources.path = $$[QT_INSTALL_EXAMPLES]/tutorial/addressbook
INSTALLS += target sources
