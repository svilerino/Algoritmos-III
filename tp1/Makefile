EJ1_DIR = ej1/
EJ2_DIR = ej2/
EJ3_DIR = ej3/
INFORME_DIR = informe/
TEST_CASES_BUILDER_DIR = common/random_testcase_builder/
CASE_TESTER_DIR = common/testing/
PLOTTING_DIR = common/plotting/

.PHONY: all clean

all: clear_screen build_modules build_informe
	@echo ""
	@echo "Todo recompilado OK."

clean: clear_screen clean_modules clean_informe

# --- Limpieza de modulos ---

clean_modules:	clean_ejercicio_1 clean_ejercicio_2 clean_ejercicio_3 clean_random_testcase clean_graphic

clean_ejercicio_1:
	@echo ""
	@echo ""
	cd $(EJ1_DIR) ; make clean

clean_ejercicio_2:
	@echo ""
	@echo ""
	cd $(EJ2_DIR) ; make clean

clean_ejercicio_3:
	@echo ""
	@echo ""
	cd $(EJ3_DIR) ; make clean

clean_informe:
	@echo ""
	@echo ""
	cd $(INFORME_DIR) ; make clean

clean_random_testcase:
	@echo ""
	@echo ""
	cd $(TEST_CASES_BUILDER_DIR) ; make clean

clean_graphic:
	cd $(PLOTTING_DIR)plots ; rm -f *.*

# --- Compilaciones de modulos ---

build_modules:	build_ejercicio_1 build_ejercicio_2 build_ejercicio_3 build_random_testcase 

build_ejercicio_1:
	@echo ""
	@echo ""
	cd $(EJ1_DIR) ; make all

build_ejercicio_2:
	@echo ""
	@echo ""
	cd $(EJ2_DIR) ; make all

build_ejercicio_3: 
	@echo ""
	@echo ""
	cd $(EJ3_DIR) ; make all

build_informe: 
	@echo ""
	@echo ""
	cd $(INFORME_DIR) ; make all

build_random_testcase:
	@echo ""
	@echo ""
	cd $(TEST_CASES_BUILDER_DIR) ; make crear_tests	

# --- Comandos ---

clear_screen:
	reset

run-tests: all
	cd $(CASE_TESTER_DIR) ; ./run_tests.sh

graphics: run-tests
	cd $(PLOTTING_DIR) ; ./plot_tests.sh