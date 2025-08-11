/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_data.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 19:03:33 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/08 18:34:36 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_DATA_H
# define SHELL_DATA_H

# include 			<termios.h>

# include 			"def.h"
# include 			"hashmap.h"
# include			"color.h"

# define PROMPT		(AQUA"minishell> "RESET)

typedef struct s_shell_data
{
	struct termios	termios_backup;
	t_hash_map		envp_map;
	int				last_status;
	int				stdin_fd;
	int				stdout_fd;
	t_boolean		in_pipe;
}	t_shell_data;

t_shell_data	*get_shell_data(void);
t_result		init_shell_data(char *envp[]);

/*
 *    [EACCES]           Search permission is denied for a component of the path prefix.
 *    [EACCES]           The required permissions (for reading and/or writing or search or executing) are denied for the given flags.
 *    [EACCES]           O_CREAT is specified, the file does not exist, and the directory in which it is to be created does not permit writing.
 *    [EACCES]           O_TRUNC is specified and write permission is denied.
 *    [EAGAIN]           path specifies the slave side of a locked pseudo-terminal device.
 *    [EDQUOT]           O_CREAT is specified, the file does not exist, and the directory in which the entry for the new file is being placed cannot be extended because the user's quota of disk blocks on the file system containing the
 *                       directory has been exhausted.
 *    [EDQUOT]           O_CREAT is specified, the file does not exist, and the user's quota of inodes on the file system on which the file is being created has been exhausted.
 *    [EEXIST]           O_CREAT and O_EXCL are specified and the file exists.
 *    [EFAULT]           Path points outside the process's allocated address space.
 *    [EINTR]            The open() operation is interrupted by a signal.
 *    [EINVAL]           The value of oflag is not valid.
 *    [EIO]              An I/O error occurs while making the directory entry or allocating the inode for O_CREAT.
 *    [EISDIR]           The named file is a directory, and the arguments specify that it is to be opened for writing.
 *    [EISDIR]           The named file is a directory, and the arguments specify that it is to be opened for executing.
 *    [ELOOP]            Too many symbolic links are encountered in translating the pathname.  This is taken to be indicative of a looping symbolic link.
 *    [EMFILE]           The process has already reached its limit for open file descriptors.
 *    [ENAMETOOLONG]     A component of a pathname exceeds {NAME_MAX} characters, or an entire path name exceeded {PATH_MAX} characters.
 *    [ENFILE]           The system file table is full.
 *    [ENOTDIR]          O_DIRECTORY was specified and the target is not a directory.
 *    [ENOTDIR]          O_SEARCH was specified and the target is not a directory.
 *    [ELOOP]            O_NOFOLLOW was specified and the target is a symbolic link.
 *    [ELOOP]            O_NOFOLLOW_ANY was specified and and a component of the path is a symbolic link.
 *    [ENOENT]           O_CREAT is not set and the named file does not exist.
 *    [ENOENT]           A component of the path name that must exist does not exist.
 *    [ENOSPC]           O_CREAT is specified, the file does not exist, and the directory in which the entry for the new file is being placed cannot be extended because there is no space left on the file system containing the directory.
 *    [ENOSPC]           O_CREAT is specified, the file does not exist, and there are no free inodes on the file system on which the file is being created.
 *    [ENOTDIR]          A component of the path prefix is not a directory.
 *    [EDEADLK]          A component of the pathname refers to a “dataless” directory that requires materialization and the I/O policy of the current thread or process disallows dataless directory materialization (see getiopolicy_np(3)).
 *    [ENXIO]            The named file is a character-special or block-special file and the device associated with this special file does not exist.
 *    [ENXIO]            O_NONBLOCK and O_WRONLY are set, the file is a FIFO, and no process has it open for reading.
 *    [EOPNOTSUPP]       O_SHLOCK or O_EXLOCK is specified, but the underlying filesystem does not support locking.
 *    [EOPNOTSUPP]       An attempt is made to open a socket (not currently implemented).
 *    [EOVERFLOW]        The named file is a regular file and its size does not fit in an object of type off_t.
 *    [EROFS]            The named file resides on a read-only file system, and the file is to be modified.
 *    [ETXTBSY]          The file is a pure procedure (shared text) file that is being executed and the open() call requests write access.
 *    [EBADF]            The path argument does not specify an absolute path and the fd argument is neither AT_FDCWD nor a valid file descriptor open for searching.
 *    [ENOTDIR]          The path argument is not an absolute path and fd is neither AT_FDCWD nor a file descriptor associated with a directory.
 *    [EILSEQ]           The filename does not match the encoding rules.
 *    [EWOULDBLOCK]      O_SHLOCK or O_EXLOCK is specified, but the file is locked and the O_NONBLOCK option was specified.
 *    [EACCES]           path does not reside in the directory hierarchy beneath the starting directory passed by fd and O_RESOLVE_BENEATH option was specified.
 */

#endif
