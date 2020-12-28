/*
 * gtk-cpuspeedy
 * http://cpuspeedy.sourceforge.net
 *
 * Copyright (C) Gabriele Giorgetti <gabriele_giorgetti@tin.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include <gtk/gtk.h>

char CPUSPEEDY_PATH[4096];
int  CPUSPEEDY_HAVE_PATH;
int  CPUSPEEDY_CHECK_VERSION;
int  CPUSPEEDY_CHECK_PATH;

gint cpuspeedy_speed (gchar *value); 
gint cpuspeedy_status (gpointer data);

void on_radiobutton_min_toggled (GtkToggleButton *togglebutton, gpointer user_data);
void on_radiobutton_low_toggled (GtkToggleButton *togglebutton, gpointer user_data);
void on_radiobutton_med_toggled (GtkToggleButton *togglebutton, gpointer user_data);
void on_radiobutton_high_toggled (GtkToggleButton *togglebutton, gpointer user_data);
void on_radiobutton_max_toggled (GtkToggleButton *togglebutton, gpointer user_data);
void on_button_about_pressed (GtkButton *button, gpointer user_data);

gint quit (GtkWidget *widget, gpointer data);
