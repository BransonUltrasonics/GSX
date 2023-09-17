import PDF_converter
import converter_gui

normal_read_prog = 0
safe_read_prog = 0
conversion_prog = 0
error_check_prog =0
concat_prog = 0
archive_prog = 0


def normal_read_tracking(curr_page, tot_pages):
    print(f"Normal Reading Page {curr_page} of {tot_pages}")
    track_to_log = PDF_converter.track_to_log
    if track_to_log:
        PDF_converter.write_to_log(f"Normal Reading Page {curr_page} of {tot_pages}")
    global normal_read_prog
    normal_read_prog = round(((curr_page / tot_pages) * 100), 2)

def get_normal_prog():
    return normal_read_prog

def safe_read_tracking(curr_page, tot_pages):
    print(f"Safe Reading Page {curr_page} of {tot_pages}")


def conversion_tracking(curr_table, tot_tables):
    print(f"Converting table {curr_table} of {tot_tables + 1}")
    global conversion_prog
    conversion_prog = round(((curr_table / tot_tables) * 100), 2)


def get_conversion_prog():
    return conversion_prog

def error_check_tracking(curr_page, tot_pages):
    print(f"Checking page {curr_page} of {tot_pages} for errors")
    global error_check_prog
    error_check_prog = round(((curr_page / tot_pages) * 100), 2)

def get_error_prog():
    return error_check_prog

def concatenation_tracker(curr_page, tot_pages):
    print(f"Concatenating page {curr_page} of {tot_pages}")
    global concat_prog
    concat_prog = round(((curr_page / tot_pages) * 100), 2)


def get_concat_prog():
    # print("returning concat\n")
    return concat_prog

def archiving_tracker(curr_page, tot_pages):
    print(f"Archiving page {curr_page} of {tot_pages}")
    global archive_prog
    archive_prog = round(((curr_page / tot_pages) * 100), 2)

def get_archiving_prog():
    # print("returning arch\n")
    return archive_prog

def get_total_prog():
    total_prog = get_normal_prog() + \
                 get_conversion_prog() + \
                 get_error_prog() + \
                 get_concat_prog() + \
                 get_archiving_prog()
    total_prog /= 5
    return total_prog

def start_converter(input_file, output_dest, file_type, desired_pages, compress_files, track_to_txt):
    PDF_converter.main(input_file, output_dest, file_type, desired_pages, compress_files, track_to_txt)
    #converter_gui.Ui_MainWindow.enable_convert_button(self)

def reset_all_progs():
    global normal_read_prog
    global conversion_prog
    global error_check_prog
    global concat_prog
    global archive_prog
    normal_read_prog = 0
    conversion_prog = 0
    error_check_prog = 0
    concat_prog = 0
    archive_prog = 0