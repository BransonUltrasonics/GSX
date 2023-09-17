
# PlanAhead Launch Script for Post-Synthesis floorplanning, created by Project Navigator

create_project -name GSX_FPGA -dir "C:/Users/76053/Documents/Entrenamientos/VHDL_FPGA/GSX_FPGA/planAhead_run_4" -part xc3s1400aft256-4
set_property design_mode GateLvl [get_property srcset [current_run -impl]]
set_property edif_top_file "C:/Users/76053/Documents/Entrenamientos/VHDL_FPGA/GSX_FPGA/FPGA_DUPS.ngc" [ get_property srcset [ current_run ] ]
add_files -norecurse { {C:/Users/76053/Documents/Entrenamientos/VHDL_FPGA/GSX_FPGA} {ipcore_dir} }
add_files [list {ipcore_dir/counter16.ncf}] -fileset [get_property constrset [current_run]]
add_files [list {ipcore_dir/counter3cr.ncf}] -fileset [get_property constrset [current_run]]
set_property target_constrs_file "FPGA_DUPS.ucf" [current_fileset -constrset]
add_files [list {FPGA_DUPS.ucf}] -fileset [get_property constrset [current_run]]
link_design
