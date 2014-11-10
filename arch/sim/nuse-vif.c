#include "nuse-vif.h"

struct nuse_vif_impl *nuse_vif[NUSE_VIF_MAX];

void
nuse_vif_read (struct nuse_vif *vif, struct SimDevice *dev)
{
  struct nuse_vif_impl *impl = nuse_vif[vif->type];
  return impl->read (vif, dev);
}

void
nuse_vif_write (struct nuse_vif *vif, struct SimDevice *dev, 
                unsigned char *data, int len)
{
  struct nuse_vif_impl *impl = nuse_vif[vif->type];
  return impl->write (vif, dev, data, len);
}

void *
nuse_vif_create (enum viftype type, const char *ifname)
{
  struct nuse_vif_impl *impl = nuse_vif[type];

  /* configure promiscus */
  if (type != NUSE_VIF_TAP)
	  nuse_set_if_promisc (ifname);

  return impl->create (ifname);
}

void
nuse_vif_delete (struct nuse_vif *vif)
{
  struct nuse_vif_impl *impl = nuse_vif[vif->type];
  return impl->delete (vif);
}

