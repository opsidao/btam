SUBDIRS += libbtam \
btamd \
btam_gui

TEMPLATE = subdirs 
CONFIG += warn_on \
          qt \
          thread  \
          ordered
