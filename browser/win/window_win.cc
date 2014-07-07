#include "browser/win/window_win.h"

#include "brightray/browser/inspectable_web_contents.h"
#include "brightray/browser/win/inspectable_web_contents_view_win.h"

#include "ui/views/layout/fill_layout.h"
#include "ui/views/widget/desktop_aura/desktop_native_widget_aura.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_delegate.h"

namespace brightray_example {

namespace {

class WidgetDelegateView : public views::WidgetDelegateView {
 public:
  WidgetDelegateView(scoped_ptr<WindowWin> window)
      : window_(window.Pass()) {
    SetLayoutManager(new views::FillLayout);
  }
  ~WidgetDelegateView() {
  }

  virtual void DeleteDelegate() OVERRIDE { delete this; }
  virtual views::View* GetContentsView() OVERRIDE { return this; }
  virtual bool CanResize() const OVERRIDE { return true; }
  virtual bool CanMaximize() const OVERRIDE { return true; }
  virtual base::string16 GetWindowTitle() const OVERRIDE { return L"Brightray Example";  }
  virtual gfx::Size GetPreferredSize() OVERRIDE { return gfx::Size(800, 600); }
  virtual gfx::Size GetMinimumSize() OVERRIDE { return gfx::Size(100, 100); }

 private:
  scoped_ptr<WindowWin> window_;

  DISALLOW_COPY_AND_ASSIGN(WidgetDelegateView);
};

}

Window* Window::Create(brightray::BrowserContext* browser_context) {
  return new WindowWin(browser_context);
}

WindowWin::WindowWin(brightray::BrowserContext* browser_context)
    : Window(browser_context),
      widget_(new views::Widget) {
  auto delegate_view = new WidgetDelegateView(make_scoped_ptr(this).Pass());

  views::Widget::InitParams params;
  params.top_level = true;
  params.native_widget = new views::DesktopNativeWidgetAura(widget_);
  params.delegate = delegate_view;
  widget_->Init(params);
  auto contents_view = static_cast<brightray::InspectableWebContentsViewWin*>(inspectable_web_contents()->GetView());
  delegate_view->AddChildView(contents_view->GetView());
  delegate_view->Layout();
  WindowReady();
}

WindowWin::~WindowWin() {
}

void WindowWin::Show() {
  widget_->Show();
}

}
