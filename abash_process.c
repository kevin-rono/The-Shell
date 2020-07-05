#include "abash_process.h"


int helper(CMD *cmdList, int status)
{
    // ------ PIPES -------
    if (cmdList->type == PIPE)
    {
        // making pipes
        int pipe_fd[2];

        // to get the input and output file descriptors
        int err = pipe(pipe_fd);

        if (err < 0)
        {
            perror("pipe error");
            exit(1);
        }

        pid_t pid1;

        // process left node
        if (cmdList->left)
        {
            // fork new process
            pid1 = fork();

            // fork failed; exit
            if (pid1 < 0)
            {
                perror("fork error");
                exit(1);
            }
            // child process
            else if (pid1 == 0)
            {
                // use the write end of the pipe
                close(pipe_fd[0]);

                // dup2 to overwrite stdout
                dup2(pipe_fd[1], STDOUT_FILENO);

                // close old pipe fds
                close(pipe_fd[1]);

                // execute first child
                int proc = helper(cmdList->left, status);
                exit(proc);
            }
        }

        pid_t pid2;

        // process right node
        if (cmdList->right)
        {
            // fork new process
            pid2 = fork();

            // fork failed; exit
            if (pid2 < 0)
            {
                perror("fork error");
                exit(1);
            }
            // child process
            else if (pid2 == 0)
            {
                // use the read end of the pipe
                close(pipe_fd[1]);

                // dup2 to overwrite stdin
                dup2(pipe_fd[0], STDIN_FILENO);

                // close old pipe fds
                close(pipe_fd[0]);

                // execute first child
                int proc = helper(cmdList->right, status);
                exit(proc);
            }
        }

        // parent process

        // close pipe fds
        close(pipe_fd[0]);
        close(pipe_fd[1]);

        signal(SIGINT, SIG_IGN);

        waitpid(pid1, &status, 0);

        signal(SIGINT, SIG_DFL);

        if (STATUS(status) == 0)
        {
            setenv("?", "0", 1);
        }
        else
        {
            setenv("?", "1", 1);
        }
        
        signal(SIGINT, SIG_IGN);

        waitpid(pid2, &status, 0);

        signal(SIGINT, SIG_DFL);

        if (STATUS(status) == 0)
        {
            setenv("?", "0", 0);
        }
        else
        {
            setenv("?", "1", 1);
        }

        return STATUS(status);
        
    }
    else if (cmdList->type == SEP_END)
    {
        pid_t pid1;
        // process left node
        if (cmdList->left)
        {
            // fork new process
            pid1 = fork();

            // fork failed; exit
            if (pid1 < 0)
            {
                perror("fork error");
                exit(1);
            }
            // child process
            else if (pid1 == 0)
            {
                // execute first child
                int proc = helper(cmdList->left, status);
                exit(proc);
            }
        }

        pid_t pid2;
        // process right node
        if (cmdList->right)
        {
            // fork new process
            pid2 = fork();

            // fork failed; exit
            if (pid2 < 0)
            {
                perror("fork error");
                exit(1);
            }
            // child process
            else if (pid2 == 0)
            {
                // execute first child
                int proc = helper(cmdList->right, status);
                exit(proc);
            }
        }

        signal(SIGINT, SIG_IGN);

        waitpid(pid1, &status, 0);

        signal(SIGINT, SIG_DFL);

        signal(SIGINT, SIG_IGN);

        waitpid(pid2, &status, 0);

        signal(SIGINT, SIG_DFL);

        if (STATUS(status) == 0)
        {
            setenv("?", "0", 1);
        }
        else
        {
            setenv("?", "1", 1);
        }

        return STATUS(status);
    }
    else if (cmdList->type == SEP_AND)
    {
        pid_t pid1;

        // process left node
        if (cmdList->left)
        {
            // fork new process
            pid1 = fork();

            // fork failed; exit
            if (pid1 < 0)
            {
                perror("fork error");
                exit(1);
            }
            // child process
            else if (pid1 == 0)
            {
                // execute first child
                int proc = helper(cmdList->left, status);
                exit(proc);
            }
        }

        signal(SIGINT, SIG_IGN);

        waitpid(pid1, &status, 0);

        signal(SIGINT, SIG_DFL);

        if (STATUS(status) == 0)
        {
            pid_t pid2; 
            // process right node
            if (cmdList->right)
            {
                // fork new process
                pid2 = fork();

                // fork failed; exit
                if (pid2 < 0)
                {
                    perror("fork error");
                    exit(1);
                }
                // child process
                else if (pid2 == 0)
                {
                    // execute first child
                    int proc = helper(cmdList->right, status);
                    exit(proc);
                }
            }
            signal(SIGINT, SIG_IGN);

            waitpid(pid2, &status, 0);

            signal(SIGINT, SIG_DFL);

            if (STATUS(status) == 0)
            {
                setenv("?", "0", 1);
            }
            else
            {
                setenv("?", "1", 1);
            }
        }
        else
        {
            setenv("?", "1", 1);
        }

        return STATUS(status);
    }
    else if (cmdList->type == SEP_OR)
    {
        pid_t pid1;

        // process left node
        if (cmdList->left)
        {
            // fork new process
            pid1 = fork();

            // fork failed; exit
            if (pid1 < 0)
            {
                perror("fork error");
                exit(1);
            }
            // child process
            else if (pid1 == 0)
            {
                // execute first child
                int proc = helper(cmdList->left, status);
                exit(proc);
            }
        }
        signal(SIGINT, SIG_IGN);

        waitpid(pid1, &status, 0);

        signal(SIGINT, SIG_DFL);

        if (STATUS(status) == 1)
        {
            pid_t pid2; 
            // process right node
            if (cmdList->right)
            {
                // fork new process
                pid2 = fork();

                // fork failed; exit
                if (pid2 < 0)
                {
                    perror("fork error");
                    exit(1);
                }
                // child process
                else if (pid2 == 0)
                {
                    // execute first child
                    int proc = helper(cmdList->right, status);
                    exit(proc);
                }
            }
            signal(SIGINT, SIG_IGN);

            waitpid(pid2, &status, 0);

            signal(SIGINT, SIG_DFL);

            if (STATUS(status) == 0)
            {
                setenv("?", "0", 1);
            }
            else
            {
                setenv("?", "1", 1);
            }
        }
        else
        {
            setenv("?", "1", 1);
        }

        return STATUS(status);
    }
    else if (cmdList->type == SEP_BG)
    {
        // ------ HERE documents -------
        if (cmdList->fromType == REDIR_HERE)
        {
            // create temp file
            FILE* temp = tmpfile();

            if (temp == NULL)
            {
                perror("tmpfile error");
                exit(1);
            } 

            // copy content into temp file
            int i = 0;

            while (cmdList->fromFile[i] != '\0')
            {
                fputc(cmdList->fromFile[i], temp); 
                i++;
            }

            // set the file pointer at the beginning of the stream. 
            rewind(temp); 

            // obtain fd
            int here_fd = fileno(temp);

            if (here_fd < 0)
            {
                perror("fileno error");
                exit(1);
            } 
            else
            {
                dup2(here_fd, STDIN_FILENO); 

                close(here_fd);
            }
        }

        // ------ APP documents -------
        if (cmdList->toType == REDIR_APP)
        {
            // open new stdout file
            int app_fd = open(cmdList->toFile, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);

            if (app_fd < 0)
            {
                perror("app open error");
                exit(1);
            } 
            else
            {
                dup2(app_fd, STDOUT_FILENO); 
                close(app_fd);
            }
        }

        // if stdin has been redirected
        // ------ REDIRECT stdin to file -------
        if (cmdList->fromType == REDIR_IN)
        {
            // open new stdin file
            int stdin_fd = open(cmdList->fromFile, O_RDONLY);

            if (stdin_fd < 0)
            {
                perror("stdin open error");
                exit(1);
            } 

            dup2(stdin_fd, STDIN_FILENO); 
            close(stdin_fd);
        }

        // if stdout has been redirected
        // ------ REDIRECT stdout to file -------
        if (cmdList->toType == REDIR_OUT)
        {
            // open new stdout file
            int stdout_fd = open(cmdList->toFile, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);

            if (stdout_fd < 0)
            {
                perror("stdout open error");
                exit(1);
            } 

            dup2(stdout_fd, STDOUT_FILENO); 
            close(stdout_fd);
        }

        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork error");
            exit(1);
        }
        else if (pid == 0)
        {
            int proc = helper(cmdList->left, status);
            exit(proc);
        }
        else
        {
            fprintf(stderr, "Backgrounded: %d\n", pid);
            helper(cmdList->right, status);
        }

        if (STATUS(status) == 0)
        {
            setenv("?", "0", 1);
        }
        else
        {
            setenv("?", "1", 1);
        }

        return STATUS(status);
    }
    else if (cmdList->type == SUBCMD)
    {
        pid_t pid1;
        // process subcmd
       
        // fork new process
        pid1 = fork();

        // fork failed; exit
        if (pid1 < 0)
        {
            perror("fork error");
            exit(1);
        }
        // child process
        else if (pid1 == 0)
        {
            // deal with local variables
            if (cmdList->nLocal > 0)
            {
                // set local values
                for (int i = 0; i < cmdList->nLocal; i++)
                {
                    setenv(cmdList->locVar[i], cmdList->locVal[i], 1);
                }
            }

            // ------ HERE documents -------
            if (cmdList->fromType == REDIR_HERE)
            {
                // create temp file
                FILE* temp = tmpfile();

                if (temp == NULL)
                {
                    perror("tmpfile error");
                    exit(1);
                } 

                // copy content into temp file
                int i = 0;

                while (cmdList->fromFile[i] != '\0')
                {
                    fputc(cmdList->fromFile[i], temp); 
                    i++;
                }

                // set the file pointer at the beginning of the stream. 
                rewind(temp); 

                // obtain fd
                int here_fd = fileno(temp);

                if (here_fd < 0)
                {
                    perror("fileno error");
                    exit(1);
                } 
                else
                {
                    dup2(here_fd, STDIN_FILENO); 

                    close(here_fd);
                }
            }

            // ------ APP documents -------
            if (cmdList->toType == REDIR_APP)
            {
                // open new stdout file
                int app_fd = open(cmdList->toFile, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);

                if (app_fd < 0)
                {
                    perror("app open error");
                    exit(1);
                } 
                else
                {
                    dup2(app_fd, STDOUT_FILENO); 
                    close(app_fd);
                }
            }

            // if stdin has been redirected
            // ------ REDIRECT stdin to file -------
            int stdin_fd = 0;
            if (cmdList->fromType == REDIR_IN)
            {
                // open new stdin file
                stdin_fd = open(cmdList->fromFile, O_RDONLY);

                if (stdin_fd < 0)
                {
                    perror("stdin open error");
                    exit(1);
                } 

                dup2(stdin_fd, STDIN_FILENO); 
                close(stdin_fd);
            }

            // if stdout has been redirected
            // ------ REDIRECT stdout to file -------
            int stdout_fd = 0;
            if (cmdList->toType == REDIR_OUT)
            {
                // open new stdout file
                stdout_fd = open(cmdList->toFile, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);

                if (stdout_fd < 0)
                {
                    perror("stdout open error");
                    exit(1);
                } 

                dup2(stdout_fd, STDOUT_FILENO); 
                close(stdout_fd);
            }
            // execute child
            int proc = helper(cmdList->left, status);
            exit(proc);
        }
        else
        {
            signal(SIGINT, SIG_IGN);

            waitpid(pid1, &status, 0);

            signal(SIGINT, SIG_DFL);
        }

        if (STATUS(status) == 0)
        {
            setenv("?", "0", 1);
        }
        else
        {
            setenv("?", "1", 1);
        }

        return STATUS(status);

    }
    // ------ SIMPLE -------
    else
    {
        // ------ dirs ------
        if (strcmp(cmdList->argv[0], "dirs") == 0)
        {
            char cwd[PATH_MAX];
            
            // ------ REDIRECT stdout to file -------
            if (cmdList->toType == REDIR_OUT)
            {
                FILE *my_dirs = fopen(cmdList->toFile, "w+");

                if (getcwd(cwd, sizeof(cwd)))
                {
                    fprintf(my_dirs, "%s\n", cwd);
                    fclose(my_dirs);
                    return 0;
                }
                else
                {
                    perror("dirs error");
                    return 1;
                }
            }
            else
            {
                if (getcwd(cwd, sizeof(cwd)))
                {
                    fprintf(stdout, "%s\n", cwd);
                    return 0;
                }
                else
                {
                    perror("dirs error");
                    return 1;
                }
            }
        }

        // ------ HERE documents -------
        int here_fd = 0;

        // ------ APP documents -------
        int app_fd = 0;

        // ------ REDIRECT stdin to file -------
        int stdin_fd = 0;

        // ------ REDIRECT stdout to file -------
        int stdout_fd = 0;

        // fork new process
        pid_t pid = fork();

        // fork failed; exit
        if (pid < 0)
        {
            perror("fork error");
            exit(1);
        }
        // child process
        else if (pid == 0)
        {
            // ------ HERE documents -------
            if (cmdList->fromType == REDIR_HERE)
            {
                // create temp file
                FILE* temp = tmpfile();

                if (temp == NULL)
                {
                    perror("tmpfile error");
                    exit(1);
                } 

                // copy content into temp file
                int i = 0;

                while (cmdList->fromFile[i] != '\0')
                {
                    fputc(cmdList->fromFile[i], temp); 
                    i++;
                }

                // set the file pointer at the beginning of the stream. 
                rewind(temp); 

                // obtain fd
                here_fd = fileno(temp);

                if (here_fd < 0)
                {
                    perror("fileno error");
                    exit(1);
                } 
                else
                {
                    dup2(here_fd, STDIN_FILENO); 

                    close(here_fd);
                }
            }

            // ------ APP documents -------
            if (cmdList->toType == REDIR_APP)
            {
                // open new stdout file
                app_fd = open(cmdList->toFile, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);

                if (app_fd < 0)
                {
                    perror("app open error");
                    exit(1);
                } 
                else
                {
                    dup2(app_fd, STDOUT_FILENO); 
                    close(app_fd);
                }
            }

            // ------ REDIRECT stdin to file -------
            if (cmdList->fromType == REDIR_IN)
            {
                // open new stdin file
                stdin_fd = open(cmdList->fromFile, O_RDONLY);

                if (stdin_fd < 0)
                {
                    perror("stdin open error");
                    exit(1);
                } 
                else
                {
                    dup2(stdin_fd, STDIN_FILENO); 
                    close(stdin_fd);
                }
            }

            // ------ REDIRECT stdout to file -------
            if (cmdList->toType == REDIR_OUT)
            {
                // open new stdout file
                stdout_fd = open(cmdList->toFile, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);

                if (stdout_fd < 0)
                {
                    perror("stdout open error");
                    exit(1);
                } 
                else
                {
                    dup2(stdout_fd, STDOUT_FILENO);
                    close(stdout_fd);
                }
                
            }
            
            // deal with local variables
            if (cmdList->nLocal > 0)
            {
                // set local values
                for (int i = 0; i < cmdList->nLocal; i++)
                {
                    setenv(cmdList->locVar[i], cmdList->locVal[i], 1);
                }
            }

            // execute command
            if ((execvp(cmdList->argv[0], cmdList->argv) == -1))
            {
                perror("Nonexistent");
                exit(1);
            }
        }
        // parent process
        else
        {
            // ------ CD -------
            if (strcmp(cmdList->argv[0], "cd") == 0)
            {
                if (cmdList->argc == 2)
                {
                    chdir(cmdList->argv[1]);
                }
                else if (cmdList->argc == 1)
                {
                    chdir(getenv("HOME"));
                }
            }

            // ------ REDIRECT stdout to file -------
            if (stdout_fd > 0)
            {
                close(stdout_fd);
            }

            // ------ APP documents -------
            if (app_fd > 0)
            {
                close(app_fd);
            }

            // ------ HERE documents -------
            if (here_fd > 0)
            {
                close(here_fd);
            }

            // ------ REDIRECT stdin to file -------
            if (stdin_fd > 0)
            {
                close(stdin_fd);
            }

            // int pid_wait = waitpid(pid, &status, 0);
            signal(SIGINT, SIG_IGN);

            waitpid(pid, &status, 0);

            signal(SIGINT, SIG_DFL);
        }

        if (STATUS(status) == 0)
        {
            setenv("?", "0", 1);
        }
        else
        {
            setenv("?", "1", 1);
        }

        return STATUS(status);
    }

}

// Execute command list CMDLIST and return status of last command executed
int process (CMD *cmdList)
{
    int status;

    // reap zombies    
    pid_t zombie = waitpid(-1, &status, WNOHANG);

    if (zombie > 0)
    {
        fprintf(stderr, "Completed: %d (%d)\n", zombie, STATUS(status));
    }


    int proc = helper(cmdList, status);

    return proc;
}
