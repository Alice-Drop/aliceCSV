/*
 * #include <stdio.h>
#include <stdlib.h>
#include <string.h>

void csvLineToList(char *line, char delimiter, char ***output_list, int *output_size) {
    char *token = strtok(line, &delimiter);
    while (token != NULL) {
        (*output_list)[*output_size] = token;
        (*output_size)++;
        token = strtok(NULL, &delimiter);
    }
}

void csvTo2dTable(FILE *file, char delimiter, char ***table, int *num_rows, int *num_cols) {
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        int len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';  // Remove newline character
        }
        int current_row = *num_rows;
        *num_rows += 1;
        *table = (char **)realloc(*table, *num_rows * sizeof(char *));
        (*table)[current_row] = NULL;
        *num_cols = 0;
        csvLineToList(line, delimiter, &(*table) + current_row, num_cols);
    }
}

void fixLineLength(char ***csv_sheet, int num_rows, int num_cols) {
    int max_length = 0;
    for (int i = 0; i < num_rows; i++) {
        if (strlen((*csv_sheet)[i]) > max_length) {
            max_length = strlen((*csv_sheet)[i]);
        }
    }

    for (int i = 0; i < num_rows; i++) {
        if (strlen((*csv_sheet)[i]) < max_length) {
            int diff = max_length - strlen((*csv_sheet)[i]);
            (*csv_sheet)[i] = (char *)realloc((*csv_sheet)[i], (max_length + 1) * sizeof(char));
            for (int j = 0; j < diff; j++) {
                strcat((*csv_sheet)[i], ",");
            }
        }
    }
}

void tableToCSV(char **sheet, int num_rows, int num_cols, char *output_path, char delimiter, char *line_break) {
    FILE *output = fopen(output_path, "w");
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            char *col = sheet[i];
            if (strchr(col, '"') != NULL) {
                fprintf(output, "\"%s\"", col);
            } else if (strchr(col, delimiter) != NULL || strchr(col, '\n') != NULL) {
                fprintf(output, "\"%s\"", col);
            } else {
                fprintf(output, "%s", col);
            }
            if (j != num_cols - 1) {
                fprintf(output, "%c", delimiter);
            }
        }
        fprintf(output, "%s", line_break);
    }
    fclose(output);
}

void fixCSV(char *path, char *output_path, char origin_delimiter, char target_delimiter, char *origin_encoding, char *target_encoding, char *target_line_break, int fix_length) {
    FILE *origin_file = fopen(path, origin_encoding);
    if (origin_file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    char **table = NULL;
    int num_rows = 0, num_cols = 0;
    csvTo2dTable(origin_file, origin_delimiter, &table, &num_rows, &num_cols);
    if (fix_length) {
        fixLineLength(&table, num_rows, num_cols);
    }
    tableToCSV(table, num_rows, num_cols, output_path, target_delimiter, target_line_break);
    fclose(origin_file);
}

int main() {
    char *path = "D:\\2024工程\\跨平台aliceCSV\\example.csv";
    char *output_path = "output.csv";
    char origin_delimiter = ',';
    char target_delimiter = ',';
    char *origin_encoding = "utf-8";
    char *target_encoding = "utf-8";
    char *target_line_break = "\n";
    int fix_length = 1;

    fixCSV(path, output_path, origin_delimiter, target_delimiter, origin_encoding, target_encoding, target_line_break, fix_length);

    return 0;
}
*/