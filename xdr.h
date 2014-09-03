#ifndef XDR_H_
# define XDR_H_

bool_t xdr_dirpath(XDR * xdrs, dirpath * objp);
bool_t xdr_mountres3_ok(XDR * xdrs, mountres3_ok * objp);
bool_t xdr_mountres3(XDR * xdrs, mountres3 * objp);

#endif /* !XDR_H_ */
