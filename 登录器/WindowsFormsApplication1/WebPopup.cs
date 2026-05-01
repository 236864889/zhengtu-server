using System;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    /// <summary>
    /// 内部弹窗网页（用 WinForms WebBrowser）
    /// </summary>
    public class WebPopup : Form
    {
        private readonly WebBrowser web;

        public WebPopup(string caption, string urlOrHtml)
        {
            this.Text = string.IsNullOrEmpty(caption) ? "在线页面" : caption;
            this.StartPosition = FormStartPosition.CenterParent;
            this.Width = 1024;
            this.Height = 720;

            web = new WebBrowser();
            web.Dock = DockStyle.Fill;

            // 基本设置
            web.ScriptErrorsSuppressed = true;
            web.IsWebBrowserContextMenuEnabled = false;
            web.WebBrowserShortcutsEnabled = true;
            web.AllowWebBrowserDrop = false;
            web.ScrollBarsEnabled = true;          // ★ 一定要开滚动条

            web.DocumentCompleted += Web_DocumentCompleted;

            this.Controls.Add(web);

            // 加载内容：支持 URL 和整段 HTML
            if (!string.IsNullOrEmpty(urlOrHtml) &&
                urlOrHtml.TrimStart().StartsWith("<"))
            {
                web.DocumentText = urlOrHtml;
            }
            else
            {
                web.Navigate(urlOrHtml ?? "about:blank");
            }
        }

        private void Web_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
        {
            try
            {
                var doc = web.Document;
                if (doc == null) return;

                // 保险起见，把 html / body 的 overflow 都改成可滚动
                var htmls = doc.GetElementsByTagName("html");
                if (htmls != null && htmls.Count > 0)
                {
                    htmls[0].Style = "overflow:auto !important;";
                }

                var bodys = doc.GetElementsByTagName("body");
                if (bodys != null && bodys.Count > 0)
                {
                    // 保留原来的 margin 等，只强制 overflow
                    string oldStyle = bodys[0].GetAttribute("style") ?? "";
                    bodys[0].SetAttribute(
                        "style",
                        oldStyle + ";overflow:auto !important;"
                    );
                }

                // 再打一段 JS，覆盖页面里自己写的 overflow:hidden
                try
                {
                    web.Document.InvokeScript("eval", new object[]
                    {
                        "try{document.documentElement.style.overflow='auto';" +
                        "document.body.style.overflow='auto';}catch(e){}"
                    });
                }
                catch { }
            }
            catch
            {
                // 出错不影响使用
            }
        }
    }
}
