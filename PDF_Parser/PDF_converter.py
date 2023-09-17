import traceback
import camelot
import PyPDF2
import os
import shutil
import csv

import controller


debug = False
track_to_stdout = True
track_to_log = False
txt_out = None

# TODO: Update Progress Tracking


def read_event_log_tables(el_file, pages, normal=True):
    """
    Description:
            This function reads the tables in an event log pdf.
            Note: This is not compatible with non-event log pdfs.
    :param pages:
            str - comma seperated page numbers.
            Example: '1,3,4' or '1,4-end' or 'all'.
    :param el_file:
            str - Name of PDF event log file to read data from
    :return tables:
            camelot.core.TableList - object of data read
    """
    tables = camelot.read_pdf(el_file,
                              flavor='stream',
                              pages=f'{pages}',
                              normal=normal,
                              # table_areas=['0,650,1028,28'])
                              table_areas=['0,775,1028,28'])
    return tables


def read_event_log_tables_safe(el_file, page_numbers):
    """
    Description:
            This function is used to 'safely' use read_event_log_tables. If there's a corrupted page in the event log,
            the non-safe function will error out without producing any tables. This function is used when the non-safe
            function fails, it iterates through each page producing tables for non-corrupted pages and flagging
            corrupted ones.
    :param el_file:
             str - Name of PDF event log file to read data from
    :param page_numbers:
            list - list of desired page numbers to be converted
    :return page_tables:
            list of each non-corrupt page's camelot.core.TableList
    :return corrupted_pages:
            list of corrupted page numbers
    :return non_corrupted_pages:
            list of non-corrupted page numbers
    """
    # Try to read each page individually, and flag any 'corrupted' pages
    page_tables = []
    corrupted_pages = []
    for page in page_numbers:
        try:
            if track_to_stdout:
                print("Safe Reading Page: " + str(page))
            if track_to_log:
                write_to_log(f"Safe Reading Page {str(page)} of {str(len(page_numbers))}")
            controller.safe_read_tracking(page, len(page_numbers))
            table = read_event_log_tables(el_file, page, normal=False)
            page_tables.append(table)
        except Exception as e:
            if track_to_stdout:
                print(f"Error reading page {page}. Error: {e}.")
            if track_to_log:
                write_to_log(f"Error reading page {page}. Error: {e}")
            if debug:
                print("Printing traceback for error on page " + page)
                traceback.print_exc()
            corrupted_pages.append(page)
    non_corrupt_pages = [x for x in page_numbers if x not in corrupted_pages]
    # print("non_corrupt_pages: " + str(non_corrupt_pages))
    return page_tables, non_corrupt_pages, corrupted_pages


def parse_pages_input(pages, num_pages):
    """
    Description:
            This function is used to read the desired pages input and return a list of pages to be read.
    :param pages:
            str - comma seperated page numbers to extract tables from.
            Example: '1,3,4' or '1,4-end' or 'all'.
    :param num_pages:
            int - Number of pages in the original PDF, obtained from get_num_pages()
    :return pages_numbers:
            list of pages to be read.
    """
    page_numbers = []
    if pages == "all":
        for i in range(1, num_pages+1):
            page_numbers.append(i)
    else:
        for r in pages.split(","):
            if "-" in r:
                a, b = r.split("-")
                if b == "end":
                    for x in range(int(a), num_pages + 1):
                        page_numbers.append(x)
                else:
                    for x in range(int(a), int(b) + 1):
                        page_numbers.append(x)
            else:
                page_numbers.append(r)
    return page_numbers


def get_num_pages(pdf_file):
    """
    Description:
            Function for getting the number of pages in a pdf
    :param pdf_file:
            str - Name of PDF file to read
    :return num_pages:
            int - Number of pages
    """
    el_pdf = PyPDF2.PdfFileReader(pdf_file)
    num_pages = el_pdf.numPages
    return num_pages


def remove_pdf_extension(pdf_file):
    """
    Description:
            Function to remove .pdf extension from file name. Used to help with output files
    :param pdf_file:
            str - Name of PDF file to remove extension from
    :return no_extension:
            str - Name of PDF file without extension
    """
    filename = os.path.basename(pdf_file)
    no_extension = filename[0:-4]
    print(no_extension)
    return no_extension


def convert_tables_to_csv(tables, no_extension, out_dir, normal):
    """
    Description:
            Function to convert data tables into CSVs. Each page of the PDF gets its own CSV file.
    :param no_extension:
            str - PDF file name with no extension, obtained from remove_pdf_extension()
    :param tables:
            camelot.core.TableList - object of data read, obtained from read_event_log_tables()
    :param out_dir:
            str - path to directory to route output to
    :return:
            N/A
    """
    # Replace with .csv
    csv_extension = no_extension + '.csv'
    # create path
    out_path = os.path.join(out_dir, csv_extension).replace("\\", "/")
    # Export each page as it's own csv
    tables.export(out_path, f='csv', normal=normal)
    # TODO: Add Tracking for normal read


def convert_tables_to_csv_safe(page_tables, no_extension, out_dir):
    """
    Description:
            This function is used when 'corrupted' pages are found in the file.
    :param page_tables:
            list - List of each page's camelot.core.TableList. Obtained from read_event_log_tables_safe()
    :param no_extension:
            str - PDF file name with no extension, obtained from remove_pdf_extension()
    :return:
            N/A
    """
    current_table = 1
    total_tables = len(page_tables)
    for table in page_tables:
        if track_to_stdout:
            print(f"Converting table {current_table} of {total_tables + 1}")
        if track_to_log:
            write_to_log(f"Converting table {current_table} of {total_tables + 1}")
        controller.conversion_tracking(current_table, total_tables)
        convert_tables_to_csv(table, no_extension, out_dir, normal=False)
        current_table += 1


def new_line_checker(no_extension, page_numbers, out_dir):
    """
    Description:
            This function checks all the CSV files before concatenating them. If an event log file has more that 35
            lines (Lines =/= Rows) then we know that something is wrong. If a page is flagged for having too many lines,
            then new_line_fixer() is called to sort the issue.
    :param no_extension:
            str - PDF file name with no extension, obtained from remove_pdf_extension()
    :param page_numbers:
            list - list of desired page numbers to be converted
    :param out_dir:
            str - path to directory to route output to
    :return:
            N/A
    """
    tracker = 1
    page_numbers_len = len(page_numbers)
    for page in page_numbers:
        if track_to_stdout:
            print(f"Checking page {page} for newline errors ( {tracker} of {page_numbers_len} ) ")
        if track_to_log:
            write_to_log(f"Checking page {tracker} of {page_numbers_len} for newline errors")
        controller.error_check_tracking(tracker, page_numbers_len)
        tracker += 1
        filename = no_extension + '-page-' + str(page) + '-table-1.csv'
        filename = os.path.join(out_dir, filename).replace("\\", "/")
        f = open(filename)
        num_lines = sum(1 for lines in f)
        if num_lines > 36:
            if track_to_stdout:
                print(f"New line error found on page {str(page)}. Fixing...")
            if track_to_log:
                write_to_log(f"New line error found on page {str(page)}. Fixing...")
            new_line_fixer(filename)


def new_line_fixer(infile):
    """
    Description:
            This function is used to fix pages that have too many lines. The problem that this function is solving is
            the one in which the text in one column of an event log crosses into/intersects with the text in a
            neighboring column. This causes the camelot PDF reader to think that the 2 (or more) columns are actually
            one. The text in the intersected column then gets put into a new line in the same cell of whatever data
            comes first. This function finds those new lines and put any data following into it's own adjacent cell.
    :param infile:
            The CSV file that is flagged to have a newline error by new_line_checker()
    :return:
            N/A
    """
    fixed_rows = []
    with open(infile,  newline='') as csv_file:
        csv_reader = csv.reader(csv_file)
        for row in csv_reader:
            items = (len(row))
            for col in range(items):
                cell = row[col]
                if "\n" in cell:
                    elements = cell.split("\n")
                    for element in elements:
                        row.append(element)
                    del row[col]
            fixed_rows.append(row)
    with open(infile, 'w', newline='') as out_file:
        writer = csv.writer(out_file, quoting=csv.QUOTE_ALL)
        writer.writerows(fixed_rows)


def concatenate_csv_files(no_extension, page_numbers, out_dir):
    """
    Description:
            This function combines all the CSVs of each page into one CSV file.
    :param no_extension:
            str - PDF file name with no extension, obtained from remove_pdf_extension()
    :param page_numbers:
            list - list of desired page numbers to be converted
    :param out_dir:
        str - path to directory to route output to
    :return:
            N/A
    """
    tracker = 1
    page_numbers_len = len(page_numbers)
    csv_extension = no_extension + '_CSV.csv'
    out_path = os.path.join(out_dir, csv_extension).replace("\\", "/")
    with open(out_path, 'w') as output:
        loop = 0
        for page in page_numbers:
            if track_to_stdout:
                print(f"Concatenating page {page} ( {tracker} of {page_numbers_len} )")
            if track_to_log:
                write_to_log(f"Concatenating page {page} ( {tracker} of {page_numbers_len} )")
            controller.concatenation_tracker(tracker, page_numbers_len)
            tracker += 1
            each_page = no_extension + '-page-' + str(page) + '-table-1.csv'
            page_path = os.path.join(out_dir, each_page).replace("\\", "/")
            f = open(page_path)
            if loop != 0:  # skip header on every page but the first
                next(f)
            for line in f:
                output.write(line)
            f.close()
            loop += 1


def folder_csv_files(no_extension, page_numbers, out_dir, compress=False):
    """
    Description:
            This function is used to put the individual page CSVs into a folder. This is useful in the case where the
            program has an issue, each page can be inspected individually for the potential cause of the issue.
    :param no_extension:
            str - PDF file name with no extension, obtained from remove_pdf_extension()
    :param page_numbers:
            list - list of desired page numbers to be converted
    :param out_dir:
            str - path to directory to route output to
    :param compress:
            boolean - Whether or not to compress the folder. Recommended for PDFs with a large amount of pages.
    :return path:
            str - path of the folder
    """
    new_dir = no_extension + ' page CSVs'
    path = os.path.join(out_dir, new_dir).replace("\\", "/")
    if os.path.isdir(path):
        delete_csv_folder(path)
    os.mkdir(path)
    tracker = 1
    page_numbers_len = len(page_numbers)
    for page in page_numbers:
        if track_to_stdout:
            print(f"'Foldering' page {page} ( {tracker} of {page_numbers_len} )")
        if track_to_log:
            write_to_log(f"'Foldering' page {page} ( {tracker} of {page_numbers_len} )")
        controller.archiving_tracker(tracker, page_numbers_len)
        tracker += 1
        each_page = no_extension + '-page-' + str(page) + '-table-1.csv'
        page_path = os.path.join(out_dir, each_page).replace("\\", "/")
        shutil.move(page_path, path)
    if compress:
        print("Compressing Folder")
        zip_path = os.path.join(out_dir, 'Compressed CSVs').replace("\\", "/")
        shutil.make_archive(zip_path, 'zip', path)
        delete_csv_folder(path)
        return zip_path
    return path


def delete_csv_folder(path):
    """
    Description:
            This function is used to delete the folder of the individual page CSVs compressed or not. This is useful for
             when you have large PDFs and want to save storage space.
    :param path:
            str - Path of the directory to delete
    :return:
            N/A
    """
    shutil.rmtree(path)


# TODO: (Optional) Add non-corrupted page error checking
def find_non_corrupt_errors(num_pages, no_extension, corrupted_pages):
    """
    Description:
            This function is used to flag any pages that weren't corrupted but still may have errors. Potential errors
            can be caused by text from one column intersecting with the text in another column, pages that have messed
             up data but still stay in a table format, and data being in the wrong column.
    :param num_pages:
            int - Number of pages in the original PDF, obtained from get_num_pages()
    :param no_extension:
            str - PDF file name with no extension, obtained from remove_pdf_extension()
    :param corrupted_pages:
            list - corrupted pages to be skipped when checking CSVs
    :return:
    """
    # Check if number of rows is too many or too few
    # Check if number of columns is too many or too few
    # check if first column isn't number
    # check if event name has number
    # check if DateTime doesn't have date
    # check if Serial numbers are correct length
    # Check if event id is a number
    # I can't think of any ways to check old, new, and comment

def open_log(output_dest):
    log_path = os.path.join(output_dest, "conversion log.txt").replace("\\", "/")
    txt = open(log_path, 'w')
    global txt_out
    txt_out = txt

def close_log():
    global txt_out
    txt_out.close()

def write_to_log(event):
    global txt_out
    txt_out.write(event + "\n")


def main(input_file, output_dest, file_type, desired_pages, compress, track_to_txt):
    print(output_dest)
    global track_to_stdout
    track_to_stdout = False
    global track_to_log
    track_to_log = track_to_txt
    if track_to_txt:
        open_log(output_dest)
    no_extension = remove_pdf_extension(input_file)
    num_pages = get_num_pages(input_file)
    page_numbers = parse_pages_input(desired_pages, num_pages)
    if file_type == "Event Log":
        el_corrupted_pages = []
        try:
            if track_to_txt:
                write_to_log("Attempting Normal Read")
            el_tables = read_event_log_tables(input_file, desired_pages)
            convert_tables_to_csv(el_tables, no_extension, output_dest, normal=True)
            if track_to_txt:
                write_to_log("Normal Read Complete")
        except Exception as exc:
            if track_to_txt:
                write_to_log(f"There was an error reading the file normally. Error: {str(exc)}")
                write_to_log("Attempting Safe Mode")
            el_page_tables, page_numbers, el_corrupted_pages = read_event_log_tables_safe(input_file, page_numbers)
            convert_tables_to_csv_safe(el_page_tables,no_extension, output_dest)
            if track_to_txt:
                write_to_log("Conversion Complete")
        finally:
            if track_to_txt:
                write_to_log("Checking for errors")
            new_line_checker(no_extension, page_numbers, output_dest)
            if track_to_txt:
                write_to_log("Checking for errors complete")
                write_to_log("Concatenating CSVs")
            concatenate_csv_files(no_extension, page_numbers, output_dest)
            if track_to_txt:
                write_to_log("Concatenation Complete")
                write_to_log("Putting CSVs into folder")
            file_path = folder_csv_files(no_extension, page_numbers, output_dest, compress)
            if track_to_txt:
                write_to_log(f"CSVs in folder\n"
                             "Process Complete\n" 
                             f"Pages Converted: {str(page_numbers)}\n"
                             f"Pages that failed to convert: {str(el_corrupted_pages)}\n"
                             f"Location of Output {output_dest}\n"
                             f"Location of each page CSV: {file_path}")
    if track_to_txt:
        close_log()
    print("done")
