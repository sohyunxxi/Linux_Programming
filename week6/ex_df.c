#include <stdio.h>
#include <sys/statvfs.h>

void print_filesystem_info(const char *path);

int main(int argc, char *argv[]) {
   
       	printf("Filesystem	1K-blocks	Used	Available	Use%%	Mounted on\n");
       	if (argc == 1) {
	      	print_filesystem_info(".");
    	} else {
	   	for (int i = 1; i < argc; i++) {
		      	print_filesystem_info(argv[i]);
	       	}
       	}
       	return 0;
}

void print_filesystem_info(const char *path) {
        struct statvfs stat;

        if (statvfs(path, &stat) == -1) {
            perror("statvfs");
            return;
        }
        unsigned long total_blocks = stat.f_blocks * stat.f_frsize / 1024; 
	unsigned long free_blocks = stat.f_bfree * stat.f_frsize / 1024; 
        unsigned long used_blocks = (stat.f_blocks - stat.f_bfree) * stat.f_frsize / 1024;
	unsigned long usage_percentage = (used_blocks * 100) / total_blocks;

        printf("%-15s %10lu %10lu %10lu %4lu%% %13s\n",
                        path, total_blocks, used_blocks, free_blocks, usage_percentage, path);
}


