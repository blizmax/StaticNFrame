--[[

Copyright (c) 2011-2014 chukong-inc.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

]]

--------------------------------
-- @module functions

--[[--

提供一组常用函数，以及对 Lua 标准库的扩展

]]

--[[--

输出格式化字符串

~~~ lua

printf("The value = %d", 100)

~~~

@param string fmt 输出格式
@param [mixed ...] 更多参数

]]
function printf(fmt, ...)
	print(string.format(tostring(fmt), ...))
end

--[[--

检查并尝试转换为数值，如果无法转换则返回 0

@param mixed value 要检查的值
@param [integer base] 进制，默认为十进制

@return number

]]
function checknumber(value, base)
	return tonumber(value, base) or 0
end

--[[--

检查并尝试转换为整数，如果无法转换则返回 0
@param mixed value 要检查的值
@return integer

]]
function checkint(value)
	return math.round(checknumber(value))
end

--[[--

检查并尝试转换为布尔值，除了 nil 和 false，其他任何值都会返回 true

@param mixed value 要检查的值

@return boolean

]]
function checkbool(value)
	return (value ~= nil and value ~= false)
end

--[[--

检查值是否是一个表格，如果不是则返回一个空表格

@param mixed value 要检查的值

@return table

]]
function checktable(value)
	if type(value) ~= "table" then value = {} end
	return value
end

--[[--

如果表格中指定 key 的值为 nil，或者输入值不是表格，返回 false，否则返回 true

@param table hashtable 要检查的表格
@param mixed key 要检查的键名

@return boolean

]]
function isset(hashtable, key)
	local t = type(hashtable)
	return (t == "table" or t == "userdata") and hashtable[key] ~= nil
end

--[[--

深度克隆一个值

~~~ lua

-- 下面的代码，t2 是 t1 的引用，修改 t2 的属性时，t1 的内容也会发生变化
local t1 = {a = 1, b = 2}
local t2 = t1
t2.b = 3    -- t1 = {a = 1, b = 3} <-- t1.b 发生变化

-- clone() 返回 t1 的副本，修改 t2 不会影响 t1
local t1 = {a = 1, b = 2}
local t2 = clone(t1)
t2.b = 3    -- t1 = {a = 1, b = 2} <-- t1.b 不受影响

~~~

@param mixed object 要克隆的值

@return mixed

]]
function clone(object)
	local lookup_table = {}
	local function _copy(object)
		if type(object) ~= "table" then
			return object
		elseif lookup_table[object] then
			return lookup_table[object]
		end
		local new_table = {}
		lookup_table[object] = new_table
		for key, value in pairs(object) do
			new_table[_copy(key)] = _copy(value)
		end
		return setmetatable(new_table, getmetatable(object))
	end
	return _copy(object)
end

--[[--

创建一个类

~~~ lua

-- 定义名为 Shape 的基础类
local Shape = class("Shape")

-- ctor() 是类的构造函数，在调用 Shape.new() 创建 Shape 对象实例时会自动执行
function Shape:ctor(shapeName)
	self.shapeName = shapeName
	printf("Shape:ctor(%s)", self.shapeName)
end

-- 为 Shape 定义个名为 draw() 的方法
function Shape:draw()
	printf("draw %s", self.shapeName)
end

--

-- Circle 是 Shape 的继承类
local Circle = class("Circle", Shape)

function Circle:ctor()
	-- 如果继承类覆盖了 ctor() 构造函数，那么必须手动调用父类构造函数
	-- 类名.super 可以访问指定类的父类
	Circle.super.ctor(self, "circle")
	self.radius = 100
end

function Circle:setRadius(radius)
	self.radius = radius
end

-- 覆盖父类的同名方法
function Circle:draw()
	printf("draw %s, raidus = %0.2f", self.shapeName, self.raidus)
end

--

local Rectangle = class("Rectangle", Shape)

function Rectangle:ctor()
	Rectangle.super.ctor(self, "rectangle")
end

--

local circle = Circle.new()             -- 输出: Shape:ctor(circle)
circle:setRaidus(200)
circle:draw()                           -- 输出: draw circle, radius = 200.00

local rectangle = Rectangle.new()       -- 输出: Shape:ctor(rectangle)
rectangle:draw()                        -- 输出: draw rectangle

~~~

### 高级用法

class() 除了定义纯 Lua 类之外，还可以从 C++ 对象继承类。

比如需要创建一个工具栏，并在添加按钮时自动排列已有的按钮，那么我们可以使用如下的代码：

~~~ lua

-- 从 cc.Node 对象派生 Toolbar 类，该类具有 cc.Node 的所有属性和行为
local Toolbar = class("Toolbar", function()
	return display.newNode() -- 返回一个 cc.Node 对象
end)

-- 构造函数
function Toolbar:ctor()
	self.buttons = {} -- 用一个 table 来记录所有的按钮
end

-- 添加一个按钮，并且自动设置按钮位置
function Toolbar:addButton(button)
	-- 将按钮对象加入 table
	self.buttons[#self.buttons + 1] = button
	
	-- 添加按钮对象到 cc.Node 中，以便显示该按钮
	-- 因为 Toolbar 是从 cc.Node 继承的，所以可以使用 addChild() 方法
	self:addChild(button)
	
	-- 按照按钮数量，调整所有按钮的位置
	local x = 0
	for _, button in ipairs(self.buttons) do
		button:setPosition(x, 0)
		-- 依次排列按钮，每个按钮之间间隔 10 点
		x = x + button:getContentSize().width + 10
	end
end

~~~

class() 的这种用法让我们可以在 C++ 对象基础上任意扩展行为。

既然是继承，自然就可以覆盖 C++ 对象的方法：

~~~ lua

function Toolbar:setPosition(x, y)
	-- 由于在 Toolbar 继承类中覆盖了 cc.Node 对象的 setPosition() 方法
	-- 所以我们要用以下形式才能调用到 cc.Node 原本的 setPosition() 方法
	getmetatable(self).setPosition(self, x, y)
	
	printf("x = %0.2f, y = %0.2f", x, y)
end

~~~

**注意:** Lua 继承类覆盖的方法并不能从 C++ 调用到。也就是说通过 C++ 代码调用这个 cc.Node 对象的 setPosition() 方法时，并不会执行我们在 Lua 中定义的 Toolbar:setPosition() 方法。

@param string classname 类名
@param [mixed super] 父类或者创建对象实例的函数

@return table

]]
function class(classname, super)
	local superType = type(super)
	local cls
	
	if superType ~= "function" and superType ~= "table" then
		superType = nil
		super = nil
	end
	
	if superType == "function" or (super and super.__ctype == 1) then
		-- inherited from native C++ Object
		cls = {}
		
		if superType == "table" then
			-- copy fields from super
			for k,v in pairs(super) do cls[k] = v end
			cls.__create = super.__create
			cls.super    = super
		else
			cls.__create = super
		cls.ctor = function() end
		end
	
		cls.__cname = classname
		cls.__ctype = 1
	
		function cls.new(...)
			local instance = cls.__create(...)
			-- copy fields from class to native object
			for k,v in pairs(cls) do instance[k] = v end
			instance.class = cls
			instance:ctor(...)
			return instance
		end
	
	else
	-- inherited from Lua Object
		if super then
			cls = {}
			setmetatable(cls, {__index = super})
			cls.super = super
		else
			cls = {ctor = function() end}
		end
	
		cls.__cname = classname
		cls.__ctype = 2 -- lua
		cls.__index = cls
		
		function cls.new(...)
			local instance = setmetatable({}, cls)
			instance.class = cls
			instance:ctor(...)
			return instance
		end
	end
	return cls
end

--[[--

如果对象是指定类或其子类的实例，返回 true，否则返回 false

~~~ lua

local Animal = class("Animal")
local Duck = class("Duck", Animal)

print(iskindof(Duck.new(), "Animal")) -- 输出 true

~~~

@param mixed obj 要检查的对象
@param string classname 类名

@return boolean

]]
function iskindof(obj, classname)
	local t = type(obj)
	local mt
	if t == "table" then
		mt = getmetatable(obj)
	elseif t == "userdata" then
		mt = tolua.getpeer(obj)
	end
	
	while mt do
		if mt.__cname == classname then
			return true
		end
		mt = mt.super
	end
	
	return false
end

--[[--

载入一个模块

import() 与 require() 功能相同，但具有一定程度的自动化特性。

假设我们有如下的目录结构：

~~~

app/
app/classes/
app/classes/MyClass.lua
app/classes/MyClassBase.lua
app/classes/data/Data1.lua
app/classes/data/Data2.lua

~~~

MyClass 中需要载入 MyClassBase 和 MyClassData。如果用 require()，MyClass 内的代码如下：

~~~ lua

local MyClassBase = require("app.classes.MyClassBase")
local MyClass = class("MyClass", MyClassBase)

local Data1 = require("app.classes.data.Data1")
local Data2 = require("app.classes.data.Data2")

~~~

假如我们将 MyClass 及其相关文件换一个目录存放，那么就必须修改 MyClass 中的 require() 命令，否则将找不到模块文件。

而使用 import()，我们只需要如下写：

~~~ lua

local MyClassBase = import(".MyClassBase")
local MyClass = class("MyClass", MyClassBase)

local Data1 = import(".data.Data1")
local Data2 = import(".data.Data2")

~~~

当在模块名前面有一个"." 时，import() 会从当前模块所在目录中查找其他模块。因此 MyClass 及其相关文件不管存放到什么目录里，我们都不再需要修改 MyClass 中的 import() 命令。这在开发一些重复使用的功能组件时，会非常方便。

我们可以在模块名前添加多个"." ，这样 import() 会从更上层的目录开始查找模块。

~

不过 import() 只有在模块级别调用（也就是没有将 import() 写在任何函数中）时，才能够自动得到当前模块名。如果需要在函数中调用 import()，那么就需要指定当前模块名：

~~~ lua

# MyClass.lua

# 这里的 ... 是隐藏参数，包含了当前模块的名字，所以最好将这行代码写在模块的第一行
local CURRENT_MODULE_NAME = ...

local function testLoad()
	local MyClassBase = import(".MyClassBase", CURRENT_MODULE_NAME)
	# 更多代码
end

~~~

@param string moduleName 要载入的模块的名字
@param [string currentModuleName] 当前模块名

@return module

]]
function import(moduleName, currentModuleName)
	local currentModuleNameParts
	local moduleFullName = moduleName
	local offset = 1
	
	while true do
		if string.byte(moduleName, offset) ~= 46 then -- .
			moduleFullName = string.sub(moduleName, offset)
			if currentModuleNameParts and #currentModuleNameParts > 0 then
				moduleFullName = table.concat(currentModuleNameParts, ".") .. "." .. moduleFullName
			end
			break
		end
		offset = offset + 1
		
		if not currentModuleNameParts then
			if not currentModuleName then
				local n,v = debug.getlocal(3, 1)
				currentModuleName = v
			end
			
			currentModuleNameParts = string.split(currentModuleName, ".")
		end
		table.remove(currentModuleNameParts, #currentModuleNameParts)
	end
	
	return require(moduleFullName)
end

--[[--

将 Lua 对象及其方法包装为一个匿名函数

在 quick-cocos2d-x 中，许多功能需要传入一个 Lua 函数做参数，然后在特定事件发生时就会调用传入的函数。例如触摸事件、帧事件等等。

~~~ lua

local MyScene = class("MyScene", function()
	return display.newScene("MyScene")
end)

function MyScene:ctor()
	self.frameTimeCount = 0
	-- 注册帧事件
	self:addNodeEventListener(cc.NODE_ENTER_FRAME_EVENT, self.onEnterFrame)
	self:scheduleUpdate()
end

function MyScene:onEnterFrame(dt)
	self.frameTimeCount = self.frameTimeCount + dt
end

~~~

上述代码执行时将出错，报告"Invalid self" ，这就是因为 C++ 无法识别 Lua 对象方法。因此在调用我们传入的 self.onEnterFrame 方法时没有提供正确的参数。

要让上述的代码正常工作，就需要使用 handler() 进行一下包装：

~~~ lua

function MyScene:ctor()
	self.frameTimeCount = 0
	-- 注册帧事件
	self:addNodeEventListener(cc.ENTER_FRAME_EVENT, handler(self, self.onEnterFrame))
	self:scheduleUpdate()
end

~~~

实际上，除了 C++ 回调 Lua 函数之外，在其他所有需要回调的地方都可以使用 handler()。

@param mixed obj Lua 对象
@param function method 对象方法

@return function

]]
function handler(obj, method)
	return function(...)
		return method(obj, ...)
	end
end


--------------------------------
-- @module math

-- start --

--------------------------------
-- 根据系统时间初始化随机数种子，让后续的 math.random() 返回更随机的值
-- @function [parent=#math] newrandomseed

-- end --

function math.mod(a, b)
	return math.fmod(a,b)
end

function math.newrandomseed()
	local rseed = tostring(os.time()):reverse()
	rseed = string.sub(rseed, 1, 9)
	math.randomseed(rseed)
	
	math.random()
end

function math.newrandom(...)
		local rseed = tostring(os.time()):reverse()
		rseed = string.sub(rseed, 1, 9)
		math.randomseed(rseed)
	
	return math.random(...)
	-- body
end

-- start --

--------------------------------
-- 对数值进行四舍五入，如果不是数值则返回 0
-- @function [parent=#math] round
-- @param number value 输入值
-- @return number#number

-- end --

function math.round(value)
	value = checknumber(value)
	return math.floor(value + 0.5)
end

-- start --

--------------------------------
-- 角度转弧度
-- @function [parent=#math] angle2radian

-- end --

function math.angle2radian(angle)
	return angle*math.pi/180
end

-- start --

--------------------------------
-- 弧度转角度
-- @function [parent=#math] radian2angle

-- end --

function math.radian2angle(radian)
	return radian/math.pi*180
end



--------------------------------
-- @module io

-- start --

--------------------------------
-- 检查指定的文件或目录是否存在，如果存在返回 true，否则返回 false
-- @function [parent=#io] exists
-- @param string path 要检查的文件或目录的完全路径
-- @return boolean#boolean

--[[--

检查指定的文件或目录是否存在，如果存在返回 true，否则返回 false

可以使用 cc.FileUtils:fullPathForFilename() 函数查找特定文件的完整路径，例如：

~~~ lua

local path = cc.FileUtils:getInstance():fullPathForFilename("gamedata.txt")
if io.exists(path) then
	....
end

~~~

]]

-- end --

 function io.exists(path)
	local file = io.open(path, "r")
	if file then
		io.close(file)
		return true
	end
	return false
end

-- start --

--------------------------------
-- 读取文件内容，返回包含文件内容的字符串，如果失败返回 nil
-- @function [parent=#io] readfile
-- @param string path 文件完全路径
-- @return string#string

--[[--

读取文件内容，返回包含文件内容的字符串，如果失败返回 nil

io.readfile() 会一次性读取整个文件的内容，并返回一个字符串，因此该函数不适宜读取太大的文件。

]]

-- end --

function io.readfile(path)
	local file = io.open(path, "r")
	if file then
		local content = file:read("*a")
		io.close(file)
		return content
	end
	return nil
end

-- start --

--------------------------------
-- 以字符串内容写入文件，成功返回 true，失败返回 false
-- @function [parent=#io] writefile
-- @param string path 文件完全路径
-- @param string content 要写入的内容
-- @param string mode 写入模式，默认值为 "w+b"
-- @return boolean#boolean

--[[--

以字符串内容写入文件，成功返回 true，失败返回 false

"mode 写入模式" 参数决定 io.writefile() 如何写入内容，可用的值如下：

-   "w+" : 覆盖文件已有内容，如果文件不存在则创建新文件
-   "a+" : 追加内容到文件尾部，如果文件不存在则创建文件

此外，还可以在 "写入模式" 参数最后追加字符 "b" ，表示以二进制方式写入数据，这样可以避免内容写入不完整。

**Android 特别提示:** 在 Android 平台上，文件只能写入存储卡所在路径，assets 和 data 等目录都是无法写入的。

]]

-- end --

function io.writefile(path, content, mode)
	mode = mode or "w+b"
	local file = io.open(path, mode)
	if file then
		if file:write(content) == nil then return false end
		io.close(file)
		return true
	else
		return false
	end
end

-- start --

--------------------------------
-- 拆分一个路径字符串，返回组成路径的各个部分
-- @function [parent=#io] pathinfo
-- @param string path 要分拆的路径字符串
-- @return table#table

--[[--

拆分一个路径字符串，返回组成路径的各个部分

~~~ lua

local pathinfo  = io.pathinfo("/var/app/test/abc.png")

-- 结果:
-- pathinfo.dirname  = "/var/app/test/"
-- pathinfo.filename = "abc.png"
-- pathinfo.basename = "abc"
-- pathinfo.extname  = ".png"

~~~

]]

-- end --

function io.pathinfo(path)
	local pos = string.len(path)
	local extpos = pos + 1
	while pos > 0 do
		local b = string.byte(path, pos)
		if b == 46 then -- 46 = char "."
			extpos = pos
		elseif b == 47 then -- 47 = char "/"
			break
		end
		pos = pos - 1
	end
	
	local dirname = string.sub(path, 1, pos)
	local filename = string.sub(path, pos + 1)
	extpos = extpos - pos
	local basename = string.sub(filename, 1, extpos - 1)
	local extname = string.sub(filename, extpos)
	return {
	dirname = dirname,
	filename = filename,
	basename = basename,
	extname = extname
	}
end

-- start --

--------------------------------
-- 返回指定文件的大小，如果失败返回 false
-- @function [parent=#io] filesize
-- @param string path 文件完全路径
-- @return integer#integer

-- end --

function io.filesize(path)
	local size = false
	local file = io.open(path, "r")
	if file then
		local current = file:seek()
		size = file:seek("end")
		file:seek("set", current)
		io.close(file)
	end
	return size
end


--------------------------------
-- @module table

-- start --

--------------------------------
-- 计算表格包含的字段数量
-- @function [parent=#table] nums
-- @param table t 要检查的表格
-- @return integer#integer

--[[--

计算表格包含的字段数量

Lua table 的 "#" 操作只对依次排序的数值下标数组有效，table.nums() 则计算 table 中所有不为 nil 的值的个数。

]]

-- end --

function table.nums(t)
	local count = 0
	for k, v in pairs(t) do
		count = count + 1
	end
	return count
end

function table.isnull(t)
	for k, v in pairs(t) do
		return false
	end
	return true
end

-- start --

--------------------------------
-- 返回指定表格中的所有键
-- @function [parent=#table] keys
-- @param table hashtable 要检查的表格
-- @return table#table

--[[--

返回指定表格中的所有键

~~~ lua

local hashtable = {a = 1, b = 2, c = 3}
local keys = table.keys(hashtable)
-- keys = {"a", "b", "c"}

~~~

]]

-- end --

function table.keys(hashtable)
	local keys = {}
	for k, v in pairs(hashtable) do
		keys[#keys + 1] = k
	end
	return keys
end

-- start --

--------------------------------
-- 返回指定表格中的所有值
-- @function [parent=#table] values
-- @param table hashtable 要检查的表格
-- @return table#table

--[[--

返回指定表格中的所有值

~~~ lua

local hashtable = {a = 1, b = 2, c = 3}
local values = table.values(hashtable)
-- values = {1, 2, 3}

~~~

]]

-- end --

function table.values(hashtable)
	local values = {}
	for k, v in pairs(hashtable) do
		values[#values + 1] = v
	end
	return values
end

-- start --

--------------------------------
-- 将来源表格中所有键及其值复制到目标表格对象中，如果存在同名键，则覆盖其值
-- @function [parent=#table] merge
-- @param table dest 目标表格
-- @param table src 来源表格

--[[--

将来源表格中所有键及其值复制到目标表格对象中，如果存在同名键，则覆盖其值

~~~ lua

local dest = {a = 1, b = 2}
local src  = {c = 3, d = 4}
table.merge(dest, src)
-- dest = {a = 1, b = 2, c = 3, d = 4}

~~~

]]

-- end --

function table.merge(dest, src)
	for k, v in pairs(src) do
		dest[k] = v
	end
end

-- start --

--------------------------------
-- 在目标表格的指定位置插入来源表格，如果没有指定位置则连接两个表格
-- @function [parent=#table] insertto
-- @param table dest 目标表格
-- @param table src 来源表格
-- @param integer begin 插入位置,默认最后

--[[--

在目标表格的指定位置插入来源表格，如果没有指定位置则连接两个表格

~~~ lua

local dest = {1, 2, 3}
local src  = {4, 5, 6}
table.insertto(dest, src)
-- dest = {1, 2, 3, 4, 5, 6}

dest = {1, 2, 3}
table.insertto(dest, src, 5)
-- dest = {1, 2, 3, nil, 4, 5, 6}

~~~

]]

-- end --

function table.insertto(dest, src, begin)
	begin = checkint(begin)
	if begin <= 0 then
		begin = #dest + 1
	end
	
	local len = #src
	for i = 0, len - 1 do
		dest[i + begin] = src[i + 1]
	end
end

-- start --

--------------------------------
-- 从表格中查找指定值，返回其索引，如果没找到返回 false
-- @function [parent=#table] indexof
-- @param table array 表格
-- @param mixed value 要查找的值
-- @param integer begin 起始索引值
-- @return integer#integer

--[[--

从表格中查找指定值，返回其索引，如果没找到返回 false

~~~ lua

local array = {"a", "b", "c"}
print(table.indexof(array, "b")) -- 输出 2

~~~

]]

-- end --

function table.indexof(array, value, begin)
	for i = begin or 1, #array do
		if array[i] == value then return i end
	end
	return false
end

-- start --

--------------------------------
-- 从表格中查找指定值，返回其 key，如果没找到返回 nil
-- @function [parent=#table] keyof
-- @param table hashtable 表格
-- @param mixed value 要查找的值
-- @return string#string  该值对应的 key

--[[--

从表格中查找指定值，返回其 key，如果没找到返回 nil

~~~ lua

local hashtable = {name = "dualface", comp = "chukong"}
print(table.keyof(hashtable, "chukong")) -- 输出 comp

~~~

]]

-- end --

function table.keyof(hashtable, value)
	for k, v in pairs(hashtable) do
		if v == value then return k end
	end
	return nil
end

-- start --

--------------------------------
-- 从表格中删除指定值，返回删除的值的个数
-- @function [parent=#table] removebyvalue
-- @param table array 表格
-- @param mixed value 要删除的值
-- @param boolean removeall 是否删除所有相同的值
-- @return integer#integer

--[[--

从表格中删除指定值，返回删除的值的个数

~~~ lua

local array = {"a", "b", "c", "c"}
print(table.removebyvalue(array, "c", true)) -- 输出 2

~~~

]]

-- end --

function table.removebyvalue(array, value, removeall)
	local c, i, max = 0, 1, #array
	while i <= max do
		if array[i] == value then
			table.remove(array, i)
			c = c + 1
			i = i - 1
			max = max - 1
			if not removeall then break end
		end
		i = i + 1
	end
	return c
end

-- start --

--------------------------------
-- 对表格中每一个值执行一次指定的函数，并用函数返回值更新表格内容
-- @function [parent=#table] map
-- @param table t 表格
-- @param function fn 函数

--[[--

对表格中每一个值执行一次指定的函数，并用函数返回值更新表格内容

~~~ lua

local t = {name = "dualface", comp = "chukong"}
table.map(t, function(v, k)
	-- 在每一个值前后添加括号
	return "[" .. v .. "]"
end)

-- 输出修改后的表格内容
for k, v in pairs(t) do
	print(k, v)
end

-- 输出
-- name [dualface]
-- comp [chukong]

~~~

fn 参数指定的函数具有两个参数，并且返回一个值。原型如下：

~~~ lua

function map_function(value, key)
	return value
end

~~~

]]

-- end --

function table.map(t, fn)
	for k, v in pairs(t) do
		t[k] = fn(v, k)
	end
end

-- start --

--------------------------------
-- 对表格中每一个值执行一次指定的函数，但不改变表格内容
-- @function [parent=#table] walk
-- @param table t 表格
-- @param function fn 函数

--[[--

对表格中每一个值执行一次指定的函数，但不改变表格内容

~~~ lua

local t = {name = "dualface", comp = "chukong"}
table.walk(t, function(v, k)
	-- 输出每一个值
	print(v)
end)

~~~

fn 参数指定的函数具有两个参数，没有返回值。原型如下：

~~~ lua

function map_function(value, key)
	
end

~~~

]]

-- end --

function table.walk(t, fn)
	for k,v in pairs(t) do
		fn(v, k)
	end
end

-- start --

--------------------------------
-- 对表格中每一个值执行一次指定的函数，如果该函数返回 false，则对应的值会从表格中删除
-- @function [parent=#table] filter
-- @param table t 表格
-- @param function fn 函数

--[[--

对表格中每一个值执行一次指定的函数，如果该函数返回 false，则对应的值会从表格中删除

~~~ lua

local t = {name = "dualface", comp = "chukong"}
table.filter(t, function(v, k)
	return v ~= "dualface" -- 当值等于 dualface 时过滤掉该值
end)

-- 输出修改后的表格内容
for k, v in pairs(t) do
	print(k, v)
end

-- 输出
-- comp chukong

~~~

fn 参数指定的函数具有两个参数，并且返回一个 boolean 值。原型如下：

~~~ lua

function map_function(value, key)
	return true or false
end

~~~

]]

-- end --

function table.filter(t, fn)
	for k, v in pairs(t) do
		if not fn(v, k) then t[k] = nil end
	end
end

-- start --

--------------------------------
-- 遍历表格，确保其中的值唯一
-- @function [parent=#table] unique
-- @param table t 表格
-- @param boolean bArray t是否是数组,是数组,t中重复的项被移除后,后续的项会前移
-- @return table#table  包含所有唯一值的新表格

--[[--

遍历表格，确保其中的值唯一

~~~ lua

local t = {"a", "a", "b", "c"} -- 重复的 a 会被过滤掉
local n = table.unique(t)

for k, v in pairs(n) do
	print(v)
end

-- 输出
-- a
-- b
-- c

~~~

]]

-- end --

function table.unique(t, bArray)
	local check = {}
	local n = {}
	local idx = 1
	for k, v in pairs(t) do
		if not check[v] then
			if bArray then
				n[idx] = v
				idx = idx + 1
			else
				n[k] = v
			end
			check[v] = true
		end
	end
	return n
end


--[[
	--从后面往前面找,找到最小的一个，把这个插入
	-- {2,5,6,8}
	--输入4
	--得到{2,4,5,6,8}
	--注意：不会对算法进行排序
]]
function table.AscInsert(t,value)
	
	table.insert(t,value)
	for i = #t, 2, -1 do
		if t[i] < t[i-1] then
			local temp = t[i]
			t[i] = t[i-1]
			t[i-1] = temp
		else
			break
		end
	end
end


--[[
	--从后面往前面找,找到最比小的一个，把这个插入
	-- {9,7,4,1}
	--输入6
	--得到{9,7,6,4,1}
	--注意：不会对算法进行排序
]]
function table.DescInsert(t,value)
	
	table.insert(t,value)
	for i = #t, 2, -1 do
		if t[i] > t[i-1] then
			local temp = t[i]
			t[i] = t[i-1]
			t[i-1] = temp
		else
			break
		end
	end
end


--------------------------------
-- @module string


string._htmlspecialchars_set = {}
--string._htmlspecialchars_set["&"] = "&amp"
string._htmlspecialchars_set["\""] = "&quot&"
string._htmlspecialchars_set["'"] = "&#039&"
string._htmlspecialchars_set["<"] = "&lt&"
string._htmlspecialchars_set[">"] = "&gt&"
string._htmlspecialchars_set["\\"] = "&slash&"

-- start --

--------------------------------
-- 将特殊字符转为 HTML 转义符
-- @function [parent=#string] htmlspecialchars
-- @param string input 输入字符串
-- @return string#string  转换结果

--[[--

将特殊字符转为 HTML 转义符

~~~ lua

print(string.htmlspecialchars("<ABC>"))
-- 输出 &lt;ABC&gt;

~~~

]]

-- end --

function string.htmlspecialchars(input)
	for k, v in pairs(string._htmlspecialchars_set) do
		input = string.gsub(input, k, v)
	end
	return input
end

-- start --

--------------------------------
-- 将 HTML 转义符还原为特殊字符，功能与 string.htmlspecialchars() 正好相反
-- @function [parent=#string] restorehtmlspecialchars
-- @param string input 输入字符串
-- @return string#string  转换结果

--[[--

将 HTML 转义符还原为特殊字符，功能与 string.htmlspecialchars() 正好相反

~~~ lua

print(string.restorehtmlspecialchars("&lt;ABC&gt;"))
-- 输出 <ABC>

~~~

]]

-- end --

function string.restorehtmlspecialchars(input)
	for k, v in pairs(string._htmlspecialchars_set) do
		input = string.gsub(input, v, k)
	end
	return input
end

-- start --

--------------------------------
-- 将utf8字符串中的特殊字符去掉，比如微信名字有些名字带表情，这个函数可以去掉这些表情
-- @function [parent=#string] nl2br
-- @param string input 输入字符串
-- @return string#string  转换结果

--[[--

将字符串中的 \n 换行符转换为 HTML 标记

~~~ lua

print(string.filterspecchars("A 没 必 要👆"))
-- 输出
-- A 没 必 要

~~~

]]

-- end --

function string.filterspecchars(s)
	if s == nil then return nil end
	local ss = {}
	local k = 1
	while true do
		if k > #s then break end
		local c = string.byte(s,k)
		if not c then break end
		if c<192 then
			if (c>=48 and c<=57) or (c>= 65 and c<=90) or (c>=97 and c<=122) then
				table.insert(ss, string.char(c))
			end
			k = k + 1
		elseif c<224 then
			k = k + 2
		elseif c<240 then
			if c>=228 and c<=233 then
				local c1 = string.byte(s,k+1)
				local c2 = string.byte(s,k+2)
				if c1 and c2 then
					local a1,a2,a3,a4 = 128,191,128,191
					if c == 228 then a1 = 184
					elseif c == 233 then a2,a4 = 190,c1 ~= 190 and 191 or 165
					end
					if c1>=a1 and c1<=a2 and c2>=a3 and c2<=a4 then
						table.insert(ss, string.char(c,c1,c2))
					end
				end
			end
			k = k + 3
		elseif c<248 then
			k = k + 4
		elseif c<252 then
			k = k + 5
		elseif c<254 then
			k = k + 6
		end
	end
	return table.concat(ss)
end

-- start --

--------------------------------
-- 将字符串中的 \n 换行符转换为 HTML 标记
-- @function [parent=#string] nl2br
-- @param string input 输入字符串
-- @return string#string  转换结果

--[[--

将字符串中的 \n 换行符转换为 HTML 标记

~~~ lua

print(string.nl2br("Hello\nWorld"))
-- 输出
-- Hello<br />World

~~~

]]

-- end --

function string.nl2br(input)
	return string.gsub(input, "\n", "<br />")
end

-- start --

--------------------------------
-- 将字符串中的特殊字符和 \n 换行符转换为 HTML 转移符和标记
-- @function [parent=#string] text2html
-- @param string input 输入字符串
-- @return string#string  转换结果

--[[--

将字符串中的特殊字符和 \n 换行符转换为 HTML 转移符和标记

~~~ lua

print(string.text2html("<Hello>\nWorld"))
-- 输出
-- &lt;Hello&gt;<br />World

~~~

]]

-- end --

function string.text2html(input)
	input = string.gsub(input, "\t", "    ")
	input = string.htmlspecialchars(input)
	input = string.gsub(input, " ", "&nbsp;")
	input = string.nl2br(input)
	return input
end

-- start --

--------------------------------
-- 用指定字符或字符串分割输入字符串，返回包含分割结果的数组
-- @function [parent=#string] split
-- @param string input 输入字符串
-- @param string delimiter 分割标记字符或字符串
-- @return array#array  包含分割结果的数组

--[[--

用指定字符或字符串分割输入字符串，返回包含分割结果的数组

~~~ lua

local input = "Hello,World"
local res = string.split(input, ",")
-- res = {"Hello", "World"}

local input = "Hello-+-World-+-Quick"
local res = string.split(input, "-+-")
-- res = {"Hello", "World", "Quick"}

~~~

]]

-- end --

function string.split(input, delimiter)
	input = tostring(input)
	delimiter = tostring(delimiter)
	if (delimiter=='') then return false end
	local pos,arr = 0, {}
	-- for each divider found
	for st,sp in function() return string.find(input, delimiter, pos, true) end do
		table.insert(arr, string.sub(input, pos, st - 1))
		pos = sp + 1
	end
	table.insert(arr, string.sub(input, pos))
	return arr
end

-- start --

--------------------------------
-- 去除输入字符串头部的空白字符，返回结果
-- @function [parent=#string] ltrim
-- @param string input 输入字符串
-- @return string#string  结果
-- @see string.rtrim, string.trim

--[[--

去除输入字符串头部的空白字符，返回结果

~~~ lua

local input = "  ABC"
print(string.ltrim(input))
-- 输出 ABC，输入字符串前面的两个空格被去掉了

~~~

空白字符包括：

-   空格
-   制表符 \t
-   换行符 \n
-   回到行首符 \r

]]

-- end --

function string.ltrim(input)
	return string.gsub(input, "^[ \t\n\r]+", "")
end

-- start --

--------------------------------
-- 去除输入字符串尾部的空白字符，返回结果
-- @function [parent=#string] rtrim
-- @param string input 输入字符串
-- @return string#string  结果
-- @see string.ltrim, string.trim

--[[--

去除输入字符串尾部的空白字符，返回结果

~~~ lua

local input = "ABC  "
print(string.rtrim(input))
-- 输出 ABC，输入字符串最后的两个空格被去掉了

~~~

]]

-- end --

function string.rtrim(input)
	return string.gsub(input, "[ \t\n\r]+$", "")
end

-- start --

--------------------------------
-- 去掉字符串首尾的空白字符，返回结果
-- @function [parent=#string] trim
-- @param string input 输入字符串
-- @return string#string  结果
-- @see string.ltrim, string.rtrim

--[[--

去掉字符串首尾的空白字符，返回结果

]]

-- end --

function string.trim(input)
	input = string.gsub(input, "^[ \t\n\r]+", "")
	return string.gsub(input, "[ \t\n\r]+$", "")
end

-- start --

--------------------------------
-- 将字符串的第一个字符转为大写，返回结果
-- @function [parent=#string] ucfirst
-- @param string input 输入字符串
-- @return string#string  结果

--[[--

将字符串的第一个字符转为大写，返回结果

~~~ lua

local input = "hello"
print(string.ucfirst(input))
-- 输出 Hello

~~~

]]

-- end --

function string.ucfirst(input)
	return string.upper(string.sub(input, 1, 1)) .. string.sub(input, 2)
end

local function urlencodechar(char)
	return "%" .. string.format("%02X", string.byte(char))
end

-- start --

--------------------------------
-- 将字符串转换为符合 URL 传递要求的格式，并返回转换结果
-- @function [parent=#string] urlencode
-- @param string input 输入字符串
-- @return string#string  转换后的结果
-- @see string.urldecode

--[[--

将字符串转换为符合 URL 传递要求的格式，并返回转换结果

~~~ lua

local input = "hello world"
print(string.urlencode(input))
-- 输出
-- hello%20world

~~~

]]

-- end --

function string.urlencode(input)
	-- convert line endings
	input = string.gsub(tostring(input), "\n", "\r\n")
	-- escape all characters but alphanumeric, '.' and '-'
	input = string.gsub(input, "([^%w%.%- ])", urlencodechar)
	-- convert spaces to "+" symbols
	return string.gsub(input, " ", "+")
end

-- start --

--------------------------------
-- 将 URL 中的特殊字符还原，并返回结果
-- @function [parent=#string] urldecode
-- @param string input 输入字符串
-- @return string#string  转换后的结果
-- @see string.urlencode

--[[--

将 URL 中的特殊字符还原，并返回结果

~~~ lua

local input = "hello%20world"
print(string.urldecode(input))
-- 输出
-- hello world

~~~

]]

-- end --

function string.urldecode(input)
	input = string.gsub (input, "+", " ")
input = string.gsub (input, "%%(%x%x)", function(h) return string.char(checknumber(h,16)) end)
input = string.gsub (input, "\r\n", "\n")
return input
end

-- start --

--------------------------------
-- 计算 UTF8 字符串的长度，每一个中文算一个字符
-- @function [parent=#string] utf8len
-- @param string input 输入字符串
-- @return integer#integer  长度

--[[--

计算 UTF8 字符串的长度，每一个中文算一个字符

~~~ lua

local input = "你好World"
print(string.utf8len(input))
-- 输出 7

~~~

]]

-- end --

function string.utf8len(input)
	local len  = string.len(input)
	local left = len
	local cnt  = 0
	local arr  = {0, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc}
	while left ~= 0 do
		local tmp = string.byte(input, -left)
		local i   = #arr
		while arr[i] do
			if tmp >= arr[i] then
				left = left - i
				break
			end
			i = i - 1
		end
		cnt = cnt + 1
	end
	return cnt
end

-- start --

--------------------------------
-- 将数值格式化为包含千分位分隔符的字符串
-- @function [parent=#string] formatnumberthousands
-- @param number num 数值
-- @return string#string  格式化结果

--[[--

将数值格式化为包含千分位分隔符的字符串

~~~ lua

print(string.formatnumberthousands(1924235))
-- 输出 1,924,235

~~~

]]

-- end --

function string.formatnumberthousands(num)
	local formatted = tostring(checknumber(num))
	local k
	while true do
		formatted, k = string.gsub(formatted, "^(-?%d+)(%d%d%d)", '%1,%2')
		if k == 0 then break end
	end
	return formatted
end

-------------------------------------
--实现C++中sleep的功能，精确到毫秒级别
--@function [parent=#float] sleep
--@ param float

--实现暂停的功能，不会消耗cpu资源，需要socket库支持"require("socket")"
-- end --

function sleep(num)
	socket.select(nil, nil, num)
end



function table.pairsByKeys (t, f)
    local a = {}
    for n in pairs(t) do table.insert(a, n) end
    table.sort(a, f)
    local i = 0                 -- iterator variable
    local iter = function ()    -- iterator function
       i = i + 1
       if a[i] == nil then return nil
       else return a[i], t[a[i]]
       end
    end
    return iter
end

function table.clone(t, meta)
  local u = {}

  if meta then
    setmetatable(u, getmetatable(t))
  end

  for i, v in pairs(t) do
    if type(v) == "table" then
      u[i] = table.clone(v)
    else
      u[i] = v
    end
  end

  return u
end


function table.keys(t)
  local keys = {}
  for k, v in pairs(t) do table.insert(keys, k) end
  return keys
end

function table.unique(t)
  local seen = {}
  for i, v in ipairs(t) do
    if not table.indexOf(seen, v) then table.insert(seen, v) end
  end

  return seen
end

function table.values(t)
  local values = {}
  for k, v in pairs(t) do table.insert(values, v) end
  return values
end

function table.last(t)
  return t[#t]
end

function table.append(t, moreValues)
  for i, v in ipairs(moreValues) do
    table.insert(t, v)
  end

  return t
end

function table.indexOf(t, value)
  for k, v in pairs(t) do
    if type(value) == "function" then
      if value(v) then return k end
    else
      if v == value then return k end
    end
  end

  return nil
end

function table.includes(t, value)
  return table.indexOf(t, value)
end

function table.removeValue(t, value)
  local index = table.indexOf(t, value)
  if index then table.remove(t, index) end
  return t
end

function table.empty(t)
  for k,v in pairs(t) do t[k]=nil end
  return t
end

function table.removeKey(table, key)
    local element = table[key]
    table[key] = nil
    return element
end

function table.each(t, func)
  for k, v in pairs(t) do
    func(v, k)
  end
end

function table.find(t, func)
  for k, v in pairs(t) do
    if func(v) then return v, k end
  end

  return nil
end

function table.filter(t, func)
  local matches = {}
  for k, v in pairs(t) do
    if func(v) then table.insert(matches, v) end
  end

  return matches
end

function table.map(t, func)
  local mapped = {}
  for k, v in pairs(t) do
    table.insert(mapped, func(v, k))
  end

  return mapped
end

function table.groupBy(t, func)
  local grouped = {}
  for k, v in pairs(t) do
    local groupKey = func(v)
    if not grouped[groupKey] then grouped[groupKey] = {} end
    table.insert(grouped[groupKey], v)
  end

  return grouped
end

function table.tostring(tbl, indent, limit, depth, jstack)
  limit   = limit  or 1000
  depth   = depth  or 7
  jstack  = jstack or {}
  local i = 0

  local output = {}
  if type(tbl) == "table" then
    -- very important to avoid disgracing ourselves with circular referencs...
    for i,t in ipairs(jstack) do
      if tbl == t then
        return "<self>,\n"
      end
    end
    table.insert(jstack, tbl)

    table.insert(output, "{\n")
    for key, value in pairs(tbl) do
      local innerIndent = (indent or " ") .. (indent or " ")
      table.insert(output, innerIndent .. tostring(key) .. " = ")
      table.insert(output,
        value == tbl and "<self>," or table.tostring(value, innerIndent, limit, depth, jstack)
      )

      i = i + 1
      if i > limit then
        table.insert(output, (innerIndent or "") .. "...\n")
        break
      end
    end

    table.insert(output, indent and (indent or "") .. "},\n" or "}")
  else
    if type(tbl) == "string" then tbl = string.format("%q", tbl) end -- quote strings
    table.insert(output, tostring(tbl) .. ",\n")
  end

  return table.concat(output)
end

function table.defaults(t,defaults)
  for k, v in pairs(defaults) do
    if t[k] == nil then t[k] = v end
  end
  return t
end

function table.slice (t,i1,i2)
  local res = {}
  local n = #t
  -- default t for range
  i1 = i1 or 1
  i2 = i2 or n
  if i2 < 0 then
    i2 = n + i2 + 1
  elseif i2 > n then
    i2 = n
  end
  if i1 < 1 or i1 > n then
    return res
  end
  local k = 1
  for i = i1,i2 do
    res[k] = t[i]
    k = k + 1
  end
  return res
end

function string.numtostring(num)

	local retStr = ""
	while  true do
		
		--if num < 100000000 then
		--	retStr = retStr..tostring(num)
		--	breank
		--end
		
		if num < 10 then
			retStr = tostring(num)..retStr
			break
		end
		
		local tem = math.mod(num, 10)
		
		num = math.floor( num/10)
		
		retStr = tostring(tem)..retStr
	end
	return retStr
end