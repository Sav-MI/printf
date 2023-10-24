int str_fmt(char *s, char *buff, int len, int buff_inx, int stage, flag *fg){
  int i, k, tmp_len;

  tmp_len = buff_inx;
  i = 0; k = 0;

  if (stage == FL_REV && len != 1){
    while (1){
      if (s[i] == ' ')
	i++;
      else
	break;
    }
    k = 1;
    while (1){
      if (s[len - k] == ' ')
	k++;
      else
	break;
    }
    len -= (i + (--k));
  }

  if (fg->prsn > len)
    fg->prsn = -1;
  len = !(fg->prsn) ? 0 : len;
  len = fg->prsn > 0 ? fg->prsn : len;

  fg->width = len < fg->width ? fg->width - len : 0;
  if (fg->width && !(fg->neg))
    buff_inx += fg->width;

  while (len){
    buff[buff_inx++] = s[i];
    i++; len--;
  }

  while (fg->width){
    if (fg->neg)
      buff[buff_inx++] = ' ';
    else
      buff[tmp_len++]= ' ';
    fg->width--;
  }
  return buff_inx;
}
