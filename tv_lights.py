import os
import sys
import time
import webbrowser

import serial
from flask import (Flask, Markup, jsonify, render_template,
                   render_template_string, request, send_file)
from infi.systray import SysTrayIcon

import tv_lights_connect as tv


def resource_path(relative_path):
    """
        Get absolute path to resource, works for dev and for PyInstaller 
        https://stackoverflow.com/questions/7674790/bundling-data-files-with-pyinstaller-onefile
    """
    try:
        # PyInstaller creates a temp folder and stores path in _MEIPASS
        base_path = sys._MEIPASS
    except Exception:
        base_path = os.path.abspath(".")

    return os.path.join(base_path, relative_path)

app = Flask('tv_lights', template_folder=str(resource_path('tv_lights_data')))


@app.route('/sendCommand/<string:command>')
def set_preset(command):
    resp = tv.sendCommandWithInitCheck(command)
    return jsonify({'resp': str(resp)})

@app.route('/exit')
def exit_program():
    exit()

@app.route('/')
def main_route():
    return render_template('index.html')


def open_control_panel(systray):
    webbrowser.open(f'http://localhost:5000/?ver={time.time()}')

def say_goodbye(systray):
    print("Goodbye!")
    os._exit(1)

menu_options = (("Open Control Panel", None, open_control_panel),)
systray = SysTrayIcon(str(resource_path(r'tv_lights_data\rainbow.ico')), "TV Lights", menu_options, on_quit=say_goodbye)
systray.start()


if __name__ == '__main__':
    app.run(host='127.0.0.1', port=5000, debug=False, use_debugger=True)