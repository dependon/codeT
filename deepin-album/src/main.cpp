/*
 * Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QScopedPointer>

#include "launcherplugin.h"

// 此文件是QML应用的启动文件，一般无需修改
// 请在LauncherPlugin::main()中实现所需功能
int main(int argc, char *argv[])
{
    LauncherPlugin plugin;
    QScopedPointer<QGuiApplication> app(plugin.createApplication(argc, argv));
    QQmlApplicationEngine engine;

    return plugin.main(app.get(), &engine);
}
