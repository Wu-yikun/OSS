# 图床解决方案

> 图床就是将你的本地图片上传到相关服务商或者个人服务器，然后获取图片对应的网络访问地址，使用者可以方便快速的将图片插入到文章中，后续图片二次使用、迁移、分享都会非常简单。

我之前常用的图床方案是使用 `Gitee` 的仓库来实现，我的博客、周刊以及一些开源电子书都用的是`Gitee`。

最近，`Gitee` 的流量审查机制锁定了我的账户，于是我的历史图片全部都无法访问了，虽然有些无奈，但我在用的时候就做了心理准备，毕竟算是违规使用其仓库资源，所以接下来将详细说下我的图床替代方案选择之路。

我对图床的基础要求就两点：**稳定 & 速度**，所以不论免费收费我都会考虑，最终得到以下方案分享给大家：

-   Github + JsDelivr
-   OSS + CDN
    -   付费：腾讯云 COS
    -   免费：Backblaze + Cloudflare
-   VPS 自建

## 💖Github + JsDelivr（推荐）

![](https://cdn.jsdelivr.net/gh/Wu-yikun/OSS/PicGo/202301111508695.png)

![](https://cdn.jsdelivr.net/gh/Wu-yikun/OSS/PicGo/202301111507003.png)

`Github` 的图床思路和 `Gitee` 是一样的，但是就目前个人使用体验来说，`Github` 的稳定性是更胜一筹的，所以我的图床一出问题，我就快速切换到这个方案应急。

配置起来也还是很方便（默认你会使用Github），首先新建一个公开仓库：

![github_pic_demo_01](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/1d2b2451ea704ee088511c144bada087~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp)

一般上传一张图片到仓库之后，就已经可以当做图床使用，如：

- 图片上传后仓库地址：[github.com/howie6879/o…](https://link.juejin.cn/?target=https%3A%2F%2Fgithub.com%2Fhowie6879%2Foss%2Fblob%2Fmaster%2Fimages%2Fwechat_howie.png "https://github.com/howie6879/oss/blob/master/images/wechat_howie.png")
- 其他用户可访问地址：[raw.githubusercontent.com/howie6879/o…](https://link.juejin.cn/?target=https%3A%2F%2Fraw.githubusercontent.com%2Fhowie6879%2Foss%2Fmaster%2Fimages%2Fwechat_howie.png "https://raw.githubusercontent.com/howie6879/oss/master/images/wechat_howie.png")
- 引入 JsDelivr 加速地址：[cdn.jsdelivr.net/gh/howie687…](https://link.juejin.cn/?target=https%3A%2F%2Fcdn.jsdelivr.net%2Fgh%2Fhowie6879%2Foss%2Fimages%2Fwechat_howie.png "https://cdn.jsdelivr.net/gh/howie6879/oss/images/wechat_howie.png")

上面同一张图片，地址的变化大家可以都点进去看看，上面说的操作都是我人工将图片上传，但是实际操作中大可不必，有很多好用的工具来自动帮我们做这件事，这里我推荐两款工具：

-   [PicGo](https://link.juejin.cn/?target=https%3A%2F%2Fgithub.com%2FMolunerfinn%2FPicGo "https://github.com/Molunerfinn/PicGo")：一个用于快速上传图片并获取图片链接的跨平台工具
-   [uPic](https://link.juejin.cn/?target=https%3A%2F%2Fgithub.com%2Fgee1k%2FuPic "https://github.com/gee1k/uPic")：功能和上面一样，纯macOS系统支持，所以在mac下面交互体验比上面流畅不少，还有对应移动端APP

上面两款工具使用方式都是一样的，针对 `Github` 做图床都需要获取 `token` 供第三方图床工具使用。

点击 [token 获取链接](https://link.juejin.cn/?target=https%3A%2F%2Fgithub.com%2Fsettings%2Ftokens%2Fnew "https://github.com/settings/tokens/new")，权限需要勾选 `repo` 和 `user`：

![github_pic_demo_02](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/ad8807af9f674a22ba4220a12bf053a5~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp)

随后在图床工具里面进行相关信息配置即可：

![github_pic_demo_03](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/a7fd354be6db4743a0e936a3ba0fbbca~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp)

至此，`Github + JsDelivr` 方案的配置使用介绍完毕，总的来说，这个方案还是比较推荐的，理由如下：

-   快速方便：只需要建立仓库配置一下接口
-   稳定，毕竟大厂商（但需要注意的是 `Github` 图片仓库过大的时候记得换仓库）
-   `JsDelivr` 解决 `Github` 在国内访问慢以及流量问题

## OSS + CDN

`OSS`（Object Storage Service）即对象存储服务，各大厂商都有对象存储服务，如腾讯的`COS`、阿里的`OSS`、华为云的`OBS`等。

如果你对图床的稳定性以及速度有比较高的要求，那么可以考虑这套方案，`OSS`的话，有免费的，也有付费的，`CDN`也是如此。

不过目前基本各大厂商都有免费额度，没有也没关系，我们博客访问量小的话每月基本上不花什么钱，我现在将笔记全部托管到`COS`且多平台同步，每月也才几毛钱。

### 腾讯云 COS

接下来我将用腾讯云`COS & CDN`服务为例，实现个人图床，其他厂商也是类似套路，就不过多介绍。

首先进入[腾讯云 COS 存储桶列表](https://link.juejin.cn/?target=https%3A%2F%2Fconsole.cloud.tencent.com%2Fcos%2Fbucket "https://console.cloud.tencent.com/cos/bucket")，点击**创建存储桶**：

![cos_pic_demo_01](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/892e2fcca3ee411fb50031dde46b2065~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp)

需要注意的是**访问权限**，切记选择**私有读写**，不允许公共访问里面的文件，为的是恶意访问的时候能减少损失。按照上图配置完成后，直接下一步就创建成功了。

为了保证安全性，这里不建议使用根用户直接进行访问，我们可以创建一个子用户来做相关写入操作。

点击[新建子用户](https://link.juejin.cn/?target=https%3A%2F%2Fconsole.cloud.tencent.com%2Fcam%2Fuser%2Fcreate "https://console.cloud.tencent.com/cam/user/create")，按照下图依次填写即可：

**选择类型**：

![](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/7348f44de6824fdf864f5126323a2efc~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp)

**填写用户信息**：

![](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/76f3fb8e9e7c46e0b3ad6e416518aadd~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp)

需要进行验证才能继续。

**设置用户权限**：

![](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/24ffdea2a0c94dda9f4cac2b62d5d020~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp)

什么都不用选择，直接继续。

**审阅信息和权限**：

![](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/f43487466c734681907dc150de18b91a~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp)

完成后会显示该子用户的 **SecretId** 和 **SecretKey**，将它们复制出来备用。

> 注：如果忘记保存，可前往用户列表->目标用户->API密钥进行获取

**为桶设置子账户**：

回到[存储桶列表](https://link.juejin.cn/?target=https%3A%2F%2Fconsole.cloud.tencent.com%2Fcos%2Fbucket "https://console.cloud.tencent.com/cos/bucket")，点击之前创建的存储桶，点击左侧的`权限管理-->存储桶访问权限`，然后点击`存储桶访问权限-->添加用户`，子账号权限设置如下：

![cos_pic_demo_02](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/dabd8e93801749dda46c7524d10a2877~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp)

至此，图床算是配置完毕，打开图床工具，将子用户的 **SecretId** 和 **SecretKey** 和相关信息录入：

![cos_pic_demo_03](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/a7053ee2cef64f858abc2e706cd7d529~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp)

可随便上传一张照片进行测试，然后打开桶列表下面的文件列表，可看到上传的照片：

![cos_pic_demo_04](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/1bc5229a130e4674b1e0cd330662a677~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp)

上传后图片的访问地址是：`https://images-***.cos.ap-guangzhou.myqcloud.com/uPic/tCyEU0.png`，但是为了防止恶意访问以及节省流量费所以我设置了私有访问，因此访问图片会提示`Access Denied`。

最后直接开启`CDN 域名`加速：

![cos_pic_demo_05](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/18c94de6c8ae4759ad330338705dd3ce~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp)

为了节省流量的费用，可以考虑在鉴权配置那设置缓存时间为一年，以及 `Referer` 名单限制访问源。

### Backblaze + Cloudflare

这个方案有以下优势：

-   每月前 10G 流量免费
-   Cloudflare 做CDN加速
-   可自定义域名

开始前，你需要有以下条件：

-   域名
-   Backblaze 账户
-   Cloudflare 账户: 按照网站提示接入域名即可

[Backblaze B2](https://link.juejin.cn/?target=https%3A%2F%2Fwww.backblaze.com%2Fb2%2Fcloud-storage.html "https://www.backblaze.com/b2/cloud-storage.html") 是一个云存储解决方案，为什么选用他呢，是因为其前10G存储是完全免费的，这用于做图床是非常够用的。

请先注册一个账号（输入邮箱就行），然后点击`Create a Bucket`，创建一个存储桶:

![b2_demo_01](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/abeb942724bb45b2b462fef37f7765e3~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp)

填写名称，记得选择`Public`权限：

![b2_demo_02](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/ae3508d2de2440c8ad667011372ee3b2~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp)

为了让第三方软件可以使用`backblaze`，接下来需要获取`Application Keys`，操作如下：

-   点击 App Keys
-   点击 Application Keys
-   填写信息进行创建

![b2_demo_03](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/4259a3eb4d1e4eada68fb76d7bb60827~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp)

当密钥创建成功，记得保存下来，因为页面关闭后就自动不再展示。

为了获取桶域名，点击`Browse Files`直接上传一张图片，上传成功后直接点击图片，会看到如下信息：

![](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/759571c077b54359bec008c998493060~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp)

提取其中`Friendly URL`显示的域名信息，比如我这里是：`https://f***.****.com/`，然后在 `Cloudflare` 解析：

![](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/e51f7c0501fb47fea88ae2df47334072~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp)

如果上一步没有添加成功，直接在域名下面的`DNS`设置解析：

![b2_demo_06](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/dcaf6d966db1461d8c364a2cbb1f9e2a~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp)

接下来点击左侧的`SSL/TLS`，设置\*\*完全(严格)\*\*模式：

![b2_demo_07](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/8bd9eb4ba1ee43f2bc1892a994b8ca02~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp)

最后在**规则**页面设置如下两个规则：

![b2_demo_08](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/7535fdbc50244ea088bb3e5f0f628334~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp)

还有一些配置需要在`Backblaze`进行设置，由于其默认不缓存，我们要先将`Bucket Settings`的`Bucket Info`添加以下配置：

```
{"cache-control": "max-age=43200000"}
复制代码
```

然后在`CORS Rules`里面设置`Share everything in this bucket with all HTTPS origins`即可。

最后，你就拥有了一个自定义域名的免费图床：

```
# 地址形式如下
https://img.turingark.com/file/howie-img/wechat_howie.png
复制代码
```

你也同样可以用`uPic`图床工具进行上传：

![b2_demo_10](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/31ed52fef09d464cb9f56e3d5a7d091f~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp)

## VPS 自建

如果你手头有服务器，那么可以考虑自建图床服务，市面上可选的图床工具还是有不少的，这里我选择 [lsky-pro](https://link.juejin.cn/?target=https%3A%2F%2Fgithub.com%2Flsky-org%2Flsky-pro "https://github.com/lsky-org/lsky-pro") 进行尝试，探索自建图床的可行性。

> 其实开源图床项目挺多的，目前看 `lsky-pro` 项目更新以及功能都算前列，而且可以选择将图片上传到腾讯云COS或者b2，因此直接选用其进行测试。

部署上手使用非常简单，直接用 [Docker 部署](https://link.juejin.cn/?target=https%3A%2F%2Fgithub.com%2FHalcyonAzure%2Flsky-pro-docker "https://github.com/HalcyonAzure/lsky-pro-docker") 即可，具体流程参考这个项目即可，最终效果如下：

![vps_lsky_01](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/7b926043ceb745e5adf817b5381f22c3~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp)

![vps_lsky_02](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/86c67e531606490c8be4c345186837cd~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp)

可以设置登录才能上传，做到权限管控，可以设定图片存储在下图任一位置：

![vps_lsky_03](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/3525a86dd17248d6b7220b701653b1d4~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp)

经过测试，使用起来还是非常方便的，最后正式使用的话建议给自己域名套上一层CDN，这块可由你自己选择把控。

## 说明

文中提到的方案都是笔者亲自试验踩坑记录而成，基于**稳定&速度**这两个前提，因此不考虑第三方图床工具，基本上是借用成熟的服务进行图床搭建，当前应该算是基本覆盖了市面上的图床方案，当然，若有更好的方案欢迎各位留言补充。

若非要我推荐一个快速简单可用的方案，我会选择 **Github + JsDelivr**，如果想自定义域名的话，我推荐 **Backblaze + Cloudflare**，如果想一劳永逸且有钱，那就直接上大厂的 OSS 服务即可。

本文相关参考资料如下：

-   [如何使用腾讯云 COS+CDN 搭建一个属于自己的图床](https://link.juejin.cn/?target=https%3A%2F%2Fr2wind.cn%2Farticles%2F20211214.html "https://r2wind.cn/articles/20211214.html")
-   [Free Image Hosting With Cloudflare Transform Rules and Backblaze B2](https://link.juejin.cn/?target=https%3A%2F%2Fwww.backblaze.com%2Fblog%2Ffree-image-hosting-with-cloudflare-transform-rules-and-backblaze-b2%2F "https://www.backblaze.com/blog/free-image-hosting-with-cloudflare-transform-rules-and-backblaze-b2/")
-   [使用 Backblaze B2 和 Cloudflare Workers 搭建免费的自定义域名图床](https://link.juejin.cn/?target=https%3A%2F%2Fblog.meow.page%2Farchives%2Ffree-personal-image-hosting-with-backblaze-b2-and-cloudflare-workers%2F "https://blog.meow.page/archives/free-personal-image-hosting-with-backblaze-b2-and-cloudflare-workers/")