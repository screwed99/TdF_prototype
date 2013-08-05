APP = TdF_prototype

include $(SDK_DIR)/Makefile.defs

OBJS = $(ASSETS).gen.o ./main.o ./Model.o ./View.o ./Controller.o
ASSETDEPS += ./assets/*.png $(ASSETS).lua

ASSETS_BUILD_PROOF := yes

include $(SDK_DIR)/Makefile.rules