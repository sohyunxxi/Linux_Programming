#include "dir_explorer.h"

int compare_by_name(const void *a, const void *b) {
    struct file_struct *fileA = (struct file_struct *)a;
    struct file_struct *fileB = (struct file_struct *)b;
    return strcmp(fileA->d_name, fileB->d_name);
}

int flag_l = false;

char* address_comp(){
    char *prompt = "";
    char buffer[1024];

    if(getcwd(buffer,sizeof(buffer))!=NULL){
	prompt = buffer; 
    }
    else
	perror("getcwd error");

    return prompt;
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF){}
}

struct file_struct set_information(const char* filename, struct stat* statbuf) {
    struct file_struct ret;
    
    strncpy(ret.d_name, filename, sizeof(ret.d_name) - 1);
    ret.d_name[sizeof(ret.d_name) - 1] = '\0';


    static short octarray[9] = {0400, 0200, 0100,
                                0040, 0020, 0010,
                                0004, 0002, 0001};

    const char *perms_list = "rwxrwxrwx";
    char perms_data[11];
    perms_data[0] = (S_ISDIR(statbuf->st_mode)) ? 'd':'-';

    for (int i = 0; i < 9; i++) {
            perms_data[i+1] = (statbuf->st_mode & octarray[i]) ? perms_list[i] : '-';
    }
    perms_data[10] = '\0';

    ret.n_link = statbuf->st_nlink;

    ret.file_size = statbuf->st_size;

    ret.pw = getpwuid(statbuf->st_uid);
    ret.gr = getgrgid(statbuf->st_gid);
    strcpy(ret.permissions, perms_data);

    ret.tm_info = localtime(&statbuf->st_mtime);
    strftime(ret.time_str, sizeof(ret.time_str), "%m-%d %H:%M:%S", ret.tm_info);    
    
    return ret;
}

void fill_list_information(struct file_struct* file_list, struct dirent *entry, int index) {
    struct stat statbuf;
    
     if (file_list == NULL) {
        fprintf(stderr, "Error: file_list is NULL.\n");
        return;
    }

    if (index < 0) {
        fprintf(stderr, "Error: index cannot be negative.\n");
        return;
    }

    if (stat(entry -> d_name , &statbuf) == -1) {
        perror("stat error");
        return;
    }
    
    file_list[index] = set_information(entry->d_name, &statbuf);
}

void change_working_dir(char* mv_dir, char* dest) {
    if(chdir(dest)!=0){
        perror("chdir error");
    }
}

void print_dir_info(struct file_struct* file_list, int file_cnt) {

    for (int i = 0; i < file_cnt; i++) {
         printf("%s[%d] ",COLOR_RESET, i+1);
         
	 if (flag_l) {
	      printf("%s ", file_list[i].permissions);
              printf("%ld ", file_list[i].n_link);
              printf("%s ", file_list[i].pw->pw_name);
              printf("%s ", file_list[i].gr->gr_name);
              printf("%lld ", file_list[i].file_size);
              printf("%s ", file_list[i].time_str);
         }
            
	 printf("%s%s%s\n", COLOR_GREEN, file_list[i].d_name, COLOR_RESET);
    }
}

void print_reg_file_info(struct file_struct* file_list, int file_cnt) {
    
    for (int i = 0; i < file_cnt; i++) {
         printf("[X] ");
            
	 if (flag_l) {
                printf("%s ", file_list[i].permissions);
                printf("%ld ",file_list[i].n_link);
                printf("%s ", file_list[i].pw->pw_name);
                printf("%s ", file_list[i].gr->gr_name);
                printf("%lld ", file_list[i].file_size);
                printf("%s ", file_list[i].time_str);
         }

         printf("%s\n", file_list[i].d_name);    
    }
}

void run(){
    char* p = strcat(address_comp(), "$ ");
    printf("%s%s%s\n",COLOR_BLUE, p, COLOR_RESET);

    struct file_struct directory_list[1023 + 1];
    struct file_struct reg_file_list[1023 + 1];

    DIR *dir;
    struct dirent *entry;
    char cwd[1024];
    int dir_count = 0;
    int reg_file_count = 0;

    strcpy(cwd, address_comp());

    dir = opendir(cwd);
    if (dir == NULL) {
        perror("opendir error");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            fill_list_information(directory_list, entry, dir_count);
            dir_count++;
        } else {
            fill_list_information(reg_file_list, entry, reg_file_count);
            reg_file_count++;
        }
    }
    
    qsort(directory_list, dir_count, sizeof(struct file_struct), compare_by_name);
    qsort(reg_file_list, reg_file_count, sizeof(struct file_struct), compare_by_name);

    print_dir_info(directory_list, dir_count);
    print_reg_file_info(reg_file_list, reg_file_count);

    int next_dir_number = 0; 
    
    printf(">>Enter directory number(cancel: -1, -l option : -2): ");
    
    if (scanf("%d", &next_dir_number) != 1) {
        perror("Invalid input. Please enter a number.");
    }

    clear_input_buffer();


    if (next_dir_number < -2 || next_dir_number > dir_count) {
        printf("Invalid directory number.\n");
	return;
    
    } else if (next_dir_number == -1) {
        exit(0);
    
    } else if (next_dir_number == -2) {
        flag_l = !flag_l;
        printf(flag_l ? "turn on the long list option\n" : "turn off the long list option\n");
        return;
    }

    char* mv_dir = cwd;
    
    change_working_dir(mv_dir, directory_list[next_dir_number-1].d_name);
    closedir(dir);
}
