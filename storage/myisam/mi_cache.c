<<<<<<< HEAD
/* Copyright (c) 2000, 2015, Oracle and/or its affiliates. All rights reserved.
   Copyright (c) 2018, Percona and/or its affiliates. All rights reserved.
   Copyright (c) 2009, 2016, MariaDB
||||||| e5d189ecb94
/* Copyright (c) 2000, 2015, Oracle and/or its affiliates. All rights reserved.
=======
/* Copyright (c) 2000, 2021, Oracle and/or its affiliates.
>>>>>>> 37b047220a907c2a6d7235ddf2b7a6be916cc82e

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.

   This program is also distributed with certain software (including
   but not limited to OpenSSL) that is licensed under separate terms,
   as designated in a particular file or component or in included license
   documentation.  The authors of MySQL hereby grant you an additional
   permission to link the program and your derivative works with the
   separately licensed software that they have included with MySQL.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License, version 2.0, for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA */

/*
  Functions for read record cacheing with myisam
  Used for reading dynamic/compressed records from datafile.

  Can fetch data directly from file (outside cache),
  if reading a small chunk straight before the cached part (with possible
  overlap).

  Can be explicitly asked not to use cache (by not setting READING_NEXT in
  flag) - useful for occasional out-of-cache reads, when the next read is
  expected to hit the cache again.

  Allows "partial read" errors in the record header (when READING_HEADER flag
  is set) - unread part is zerofilled.

  Note: out-of-cache reads are enabled for shared IO_CACHE's too,
  as these reads will be cached by OS cache (and mysql_file_pread is always atomic)
*/


#include "myisamdef.h"

int _mi_read_cache(IO_CACHE *info, uchar *buff, my_off_t pos, uint length,
		   int flag)
{
  uint read_length,in_buff_length;
  my_off_t offset;
  uchar *in_buff_pos;
  DBUG_ENTER("_mi_read_cache");
  DBUG_ASSERT(!(info->myflags & MY_ENCRYPT));

  if (pos < info->pos_in_file)
  {
    read_length=length;
    if ((my_off_t) read_length > (my_off_t) (info->pos_in_file-pos))
      read_length=(uint) (info->pos_in_file-pos);
    info->seek_not_done=1;
    if (mysql_file_pread(info->file, buff, read_length, pos, MYF(MY_NABP)))
      DBUG_RETURN(1);
    if (!(length-=read_length))
      DBUG_RETURN(0);
    pos+=read_length;
    buff+=read_length;
  }
  if (pos >= info->pos_in_file &&
      (offset= (my_off_t) (pos - info->pos_in_file)) <
      (my_off_t) (info->read_end - info->request_pos))
  {
    in_buff_pos=info->request_pos+(uint) offset;
    in_buff_length= MY_MIN(length, (size_t) (info->read_end-in_buff_pos));
    memcpy(buff,info->request_pos+(uint) offset,(size_t) in_buff_length);
    if (!(length-=in_buff_length))
      DBUG_RETURN(0);
    pos+=in_buff_length;
    buff+=in_buff_length;
  }
  else
    in_buff_length=0;
  if (flag & READING_NEXT)
  {
    if (pos != (info->pos_in_file +
		(uint) (info->read_end - info->request_pos)))
    {
      info->pos_in_file=pos;				/* Force start here */
      info->read_pos=info->read_end=info->request_pos;	/* Everything used */
      info->seek_not_done=1;
    }
    else
      info->read_pos=info->read_end;			/* All block used */
    if (!_my_b_read(info, buff, length))
      DBUG_RETURN(0);
    read_length=info->error;
  }
  else
  {
    info->seek_not_done=1;
    if ((read_length= mysql_file_pread(info->file, buff, length, pos,
                                       MYF(0))) == length)
      DBUG_RETURN(0);
  }
  if (!(flag & READING_HEADER) || (int) read_length == -1 ||
      read_length+in_buff_length < 3)
  {
    DBUG_PRINT("error",
               ("Error %d reading next-multi-part block (Got %d bytes)",
                my_errno(), (int) read_length));
    if (!my_errno() || my_errno() == -1)
      set_my_errno(HA_ERR_WRONG_IN_RECORD);
    DBUG_RETURN(1);
  }
  memset(buff+read_length, 0,
         MI_BLOCK_INFO_HEADER_LENGTH - in_buff_length - read_length);
  DBUG_RETURN(0);
} /* _mi_read_cache */
