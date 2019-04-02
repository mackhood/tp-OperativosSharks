sharksCommons: init_deps install_lib make_projects

init_deps:
	@echo "Dependencias de "
	@git submodule init && git submodule update
	@echo "Instalar SO-COMMONS-LIBRARY"
	@cd commons && sudo $(MAKE) install
	@echo "Instalar fileSystem"
	@cd file-system && sudo $(MAKE) install

install_lib:
	@echo "Instalando SHARKS shared library"
	@cd sharksCommons/sharksCommons && sudo $(MAKE) install

make_projects:
	@echo "Construyendo el Mercado de Jugadores ..."
	@cd MDJFileSystem && $(MAKE)
	@echo "Construyendo al Diego ..."
	@cd DMA && $(MAKE)
	@echo "Construyendo cpu ... "
	@cd cpu && $(MAKE)
	@echo "Construyendo S-AFA ..."
	@cd safa && $(MAKE)
	@echo "Construyendo a FunesMemory ..."
	@cd FM9 && $(MAKE)
	@echo "Corrupt DTp Listo!"

.PHONY: sharksCommons init_deps install_lib make_projects

