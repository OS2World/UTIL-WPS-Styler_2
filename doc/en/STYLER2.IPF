:userdoc.
:docprof toc=12345.
:title.Styler/2 User's Guide

.* htmlsource = index.htm
:h1 res=1 x=left y=bottom width=100% height=100%.Styler&slr.2 User&csq.s Guide
.br
.br
:lines align=center.
:font facename='Helv' size=32x20.
Styler&slr.2 User&csq.s Guide
:font facename=default size=0x0.
:font facename='Helv' size=16x10.:hp2.
version 1&per.7&per.5
:font facename=default size=0x0.:ehp2.
by Alessandro Cantatore
.* with revisions by Alexander Taylor
:elines.
:p.
[22 January 2016]
:ul compact.
.br
:li.
:link reftype=hd res=2.License agreement:elink.
:li.
:link reftype=hd res=3.Introduction:elink.
:li.
:link reftype=hd res=4.What&csq.s new:elink.
:li.
:link reftype=hd res=5.Installation-uninstallation:elink.
:li.
:link reftype=hd res=6.Features and settings:elink.
:li.
:link reftype=hd res=21.Known problems:elink.
:li.
:link reftype=hd res=22.Planned features:elink.
:eul.
.br

:h1 res=2 x=left y=bottom width=100% height=100%.License agreement
.ce GNU GENERAL PUBLIC LICENSE
.ce Version 3, 29 June 2007

:p.Copyright (C) 2007 Free Software Foundation, Inc.
<:hp2.http&colon.//fsf.org/:ehp2.>.
.br
:p.Everyone is permitted to copy and distribute verbatim copies of this license
document, but changing it is not allowed.
:p.
.ce Preamble

:p.The GNU General Public License is a free, copyleft license for software and
other kinds of works.

:p.The licenses for most software and other practical works are designed to
take away your freedom to share and change the works. By contrast, the GNU
General Public License is intended to guarantee your freedom to share and
change all versions of a program--to make sure it remains free software for all
its users. We, the Free Software Foundation, use the GNU General Public License
for most of our software; it applies also to any other work released this way
by its authors. You can apply it to your programs, too.

:p.When we speak of free software, we are referring to freedom, not price. Our
General Public Licenses are designed to make sure that you have the freedom to
distribute copies of free software (and charge for them if you wish), that you
receive source code or can get it if you want it, that you can change the
software or use pieces of it in new free programs, and that you know you can do
these things.

:p.To protect your rights, we need to prevent others from denying you these
rights or asking you to surrender the rights. Therefore, you have certain
responsibilities if you distribute copies of the software, or if you modify
it&colon. responsibilities to respect the freedom of others.

:p.For example, if you distribute copies of such a program, whether gratis or
for a fee, you must pass on to the recipients the same freedoms that you
received. You must make sure that they, too, receive or can get the source
code. And you must show them these terms so they know their rights.

:p.Developers that use the GNU GPL protect your rights with two steps&colon.
(1) assert copyright on the software, and (2) offer you this License giving you
legal permission to copy, distribute and/or modify it.

:p.For the developers' and authors' protection, the GPL clearly explains that
there is no warranty for this free software. For both users' and authors' sake,
the GPL requires that modified versions be marked as changed, so that their
problems will not be attributed erroneously to authors of previous versions.

:p.Some devices are designed to deny users access to install or run modified
versions of the software inside them, although the manufacturer can do so. This
is fundamentally incompatible with the aim of protecting users' freedom to
change the software. The systematic pattern of such abuse occurs in the area of
products for individuals to use, which is precisely where it is most
unacceptable. Therefore, we have designed this version of the GPL to prohibit
the practice for those products. If such problems arise substantially in other
domains, we stand ready to extend this provision to those domains in future
versions of the GPL, as needed to protect the freedom of users.

:p.Finally, every program is threatened constantly by software patents. States
should not allow patents to restrict development and use of software on
general-purpose computers, but in those that do, we wish to avoid the special
danger that patents applied to a free program could make it effectively
proprietary. To prevent this, the GPL assures that patents cannot be used to
render the program non-free.

:p.The precise terms and conditions for copying, distribution and modification
follow.
:p.
.ce TERMS AND CONDITIONS

:p.0. Definitions.

:p."This License" refers to version 3 of the GNU General Public License.

:p."Copyright" also means copyright-like laws that apply to other kinds of
works, such as semiconductor masks.

:p."The Program" refers to any copyrightable work licensed under this License.
Each licensee is addressed as "you". "Licensees" and recipients may be
individuals or organizations.

:p.To "modify" a work means to copy from or adapt all or part of the work in a
fashion requiring copyright permission, other than the making of an exact copy.
The resulting work is called a ?modified version? of the earlier work or a work
"based on" the earlier work.

:p.A "covered work" means either the unmodified Program or a work based on the
Program.

:p.To "propagate" a work means to do anything with it that, without permission,
would make you directly or secondarily liable for infringement under applicable
copyright law, except executing it on a computer or modifying a private copy.
Propagation includes copying, distribution (with or without modification),
making available to the public, and in some countries other activities as well.

:p.To "convey" a work means any kind of propagation that enables other parties
to make or receive copies. Mere interaction with a user through a computer
network, with no transfer of a copy, is not conveying.

:p.An interactive user interface displays "Appropriate Legal Notices" to the
extent that it includes a convenient and prominently visible feature that (1)
displays an appropriate copyright notice, and (2) tells the user that there is
no warranty for the work (except to the extent that warranties are provided),
that licensees may convey the work under this License, and how to view a copy
of this License. If the interface presents a list of user commands or options,
such as a menu, a prominent item in the list meets this criterion.

:p.1. Source Code.

:p.The "source code" for a work means the preferred form of the work for
making modifications to it. "Object code" means any non-source form of a work.

:p.A "Standard Interface" means an interface that either is an official
standard defined by a recognized standards body, or, in the case of interfaces
specified for a particular programming language, one that is widely used among
developers working in that language.

:p.The "System Libraries" of an executable work include anything, other than
the work as a whole, that (a) is included in the normal form of packaging a
Major Component, but which is not part of that Major Component, and (b) serves
only to enable use of the work with that Major Component, or to implement a
Standard Interface for which an implementation is available to the public in
source code form. A "Major Component", in this context, means a major essential
component (kernel, window system, and so on) of the specific operating system
(if any) on which the executable work runs, or a compiler used to produce the
work, or an object code interpreter used to run it.

:p.The "Corresponding Source" for a work in object code form means all the
source code needed to generate, install, and (for an executable work) run the
object code and to modify the work, including scripts to control those
activities. However, it does not include the work's System Libraries, or
general-purpose tools or generally available free programs which are used
unmodified in performing those activities but which are not part of the work.
For example, Corresponding Source includes interface definition files
associated with source files for the work, and the source code for shared
libraries and dynamically linked subprograms that the work is specifically
designed to require, such as by intimate data communication or control flow
between those subprograms and other parts of the work.

:p.The Corresponding Source need not include anything that users can regenerate
automatically from other parts of the Corresponding Source.

:p.The Corresponding Source for a work in source code form is that same work.

:p.2. Basic Permissions.

:p.All rights granted under this License are granted for the term of copyright
on the Program, and are irrevocable provided the stated conditions are met.
This License explicitly affirms your unlimited permission to run the unmodified
Program. The output from running a covered work is covered by this License only
if the output, given its content, constitutes a covered work. This License
acknowledges your rights of fair use or other equivalent, as provided by
copyright law.

:p.You may make, run and propagate covered works that you do not convey,
without conditions so long as your license otherwise remains in force. You may
convey covered works to others for the sole purpose of having them make
modifications exclusively for you, or provide you with facilities for running
those works, provided that you comply with the terms of this License in
conveying all material for which you do not control copyright. Those thus
making or running the covered works for you must do so exclusively on your
behalf, under your direction and control, on terms that prohibit them from
making any copies of your copyrighted material outside their relationship with
you.

:p.Conveying under any other circumstances is permitted solely under the
conditions stated below. Sublicensing is not allowed; section 10 makes it
unnecessary.

:p.3. Protecting Users' Legal Rights From Anti-Circumvention Law.

:p.No covered work shall be deemed part of an effective technological measure
under any applicable law fulfilling obligations under article 11 of the WIPO
copyright treaty adopted on 20 December 1996, or similar laws prohibiting or
restricting circumvention of such measures.

:p.When you convey a covered work, you waive any legal power to forbid
circumvention of technological measures to the extent such circumvention is
effected by exercising rights under this License with respect to the covered
work, and you disclaim any intention to limit operation or modification of the
work as a means of enforcing, against the work's users, your or third parties'
legal rights to forbid circumvention of technological measures.

:p.4. Conveying Verbatim Copies.

:p.You may convey verbatim copies of the Program's source code as you receive
it, in any medium, provided that you conspicuously and appropriately publish on
each copy an appropriate copyright notice; keep intact all notices stating that
this License and any non-permissive terms added in accord with section 7 apply
to the code; keep intact all notices of the absence of any warranty; and give
all recipients a copy of this License along with the Program.

:p.You may charge any price or no price for each copy that you convey, and you
may offer support or warranty protection for a fee.

:p.5. Conveying Modified Source Versions.

:p.You may convey a work based on the Program, or the modifications to produce
it from the Program, in the form of source code under the terms of section 4,
provided that you also meet all of these conditions&colon.
:dl tsize=3 break=none.
:dt.a)
:dd.The work must carry prominent notices stating that you modified it, and
giving a relevant date.
:dt.b)
:dd.The work must carry prominent notices stating that it is released under
this License and any conditions added under section 7. This requirement
modifies the requirement in section 4 to "keep intact all notices".
:dt.c)
:dd.You must license the entire work, as a whole, under this License to anyone
who comes into possession of a copy. This License will therefore apply, along
with any applicable section 7 additional terms, to the whole of the work, and
all its parts, regardless of how they are packaged. This License gives no
permission to license the work in any other way, but it does not invalidate
such permission if you have separately received it.
:dt.d)
:dd.If the work has interactive user interfaces, each must display Appropriate
Legal Notices; however, if the Program has interactive interfaces that do not
display Appropriate Legal Notices, your work need not make them do so.
:edl.

:p.A compilation of a covered work with other separate and independent works,
which are not by their nature extensions of the covered work, and which are not
combined with it such as to form a larger program, in or on a volume of a
storage or distribution medium, is called an "aggregate" if the compilation and
its resulting copyright are not used to limit the access or legal rights of the
compilation's users beyond what the individual works permit. Inclusion of a
covered work in an aggregate does not cause this License to apply to the other
parts of the aggregate.

:p.6. Conveying Non-Source Forms.

:p.You may convey a covered work in object code form under the terms of
sections 4 and 5, provided that you also convey the machine-readable
Corresponding Source under the terms of this License, in one of these ways:
:dl tsize=3 break=none.
:dt.a)
:dd.Convey the object code in, or embodied in, a physical product (including a
physical distribution medium), accompanied by the Corresponding Source fixed on
a durable physical medium customarily used for software interchange.
:dt.b)
:dd.Convey the object code in, or embodied in, a physical product (including a
physical distribution medium), accompanied by a written offer, valid for at
least three years and valid for as long as you offer spare parts or customer
support for that product model, to give anyone who possesses the object code
either (1) a copy of the Corresponding Source for all the software in the
product that is covered by this License, on a durable physical medium
customarily used for software interchange, for a price no more than your
reasonable cost of physically performing this conveying of source, or (2)
access to copy the Corresponding Source from a network server at no charge.
:dt.c)
:dd.Convey individual copies of the object code with a copy of the written
offer to provide the Corresponding Source. This alternative is allowed only
occasionally and noncommercially, and only if you received the object code with
such an offer, in accord with subsection 6b.
:dt.d)
:dd.Convey the object code by offering access from a designated place (gratis
or for a charge), and offer equivalent access to the Corresponding Source in
the same way through the same place at no further charge. You need not require
recipients to copy the Corresponding Source along with the object code. If the
place to copy the object code is a network server, the Corresponding Source may
be on a different server (operated by you or a third party) that supports
equivalent copying facilities, provided you maintain clear directions next to
the object code saying where to find the Corresponding Source. Regardless of
what server hosts the Corresponding Source, you remain obligated to ensure that
it is available for as long as needed to satisfy these requirements.
:dt.e)
:dd.Convey the object code using peer-to-peer transmission, provided you inform
other peers where the object code and Corresponding Source of the work are
being offered to the general public at no charge under subsection 6d.
:edl.

:p.A separable portion of the object code, whose source code is excluded from
the Corresponding Source as a System Library, need not be included in conveying
the object code work.

:p.A "User Product" is either (1) a "consumer product", which means any
tangible personal property which is normally used for personal, family, or
household purposes, or (2) anything designed or sold for incorporation into a
dwelling. In determining whether a product is a consumer product, doubtful
cases shall be resolved in favor of coverage. For a particular product received
by a particular user, "normally used" refers to a typical or common use of that
class of product, regardless of the status of the particular user or of the way
in which the particular user actually uses, or expects or is expected to use,
the product. A product is a consumer product regardless of whether the product
has substantial commercial, industrial or non-consumer uses, unless such uses
represent the only significant mode of use of the product.

:p."Installation Information" for a User Product means any methods, procedures,
authorization keys, or other information required to install and execute
modified versions of a covered work in that User Product from a modified
version of its Corresponding Source. The information must suffice to ensure
that the continued functioning of the modified object code is in no case
prevented or interfered with solely because modification has been made.

:p.If you convey an object code work under this section in, or with, or
specifically for use in, a User Product, and the conveying occurs as part of a
transaction in which the right of possession and use of the User Product is
transferred to the recipient in perpetuity or for a fixed term (regardless of
how the transaction is characterized), the Corresponding Source conveyed under
this section must be accompanied by the Installation Information. But this
requirement does not apply if neither you nor any third party retains the
ability to install modified object code on the User Product (for example, the
work has been installed in ROM).

:p.The requirement to provide Installation Information does not include a
requirement to continue to provide support service, warranty, or updates for a
work that has been modified or installed by the recipient, or for the User
Product in which it has been modified or installed. Access to a network may be
denied when the modification itself materially and adversely affects the
operation of the network or violates the rules and protocols for communication
across the network.

:p.Corresponding Source conveyed, and Installation Information provided, in
accord with this section must be in a format that is publicly documented (and
with an implementation available to the public in source code form), and must
require no special password or key for unpacking, reading or copying.

:p.7. Additional Terms.

:p."Additional permissions" are terms that supplement the terms of this
License by making exceptions from one or more of its conditions. Additional
permissions that are applicable to the entire Program shall be treated as
though they were included in this License, to the extent that they are valid
under applicable law. If additional permissions apply only to part of the
Program, that part may be used separately under those permissions, but the
entire Program remains governed by this License without regard to the
additional permissions.

:p.When you convey a copy of a covered work, you may at your option remove any
additional permissions from that copy, or from any part of it. (Additional
permissions may be written to require their own removal in certain cases when
you modify the work.) You may place additional permissions on material, added
by you to a covered work, for which you have or can give appropriate copyright
permission.

:p.Notwithstanding any other provision of this License, for material you add to
a covered work, you may (if authorized by the copyright holders of that
material) supplement the terms of this License with terms:
:dl tsize=3 break=none.
:dt.a)
:dd.Disclaiming warranty or limiting liability differently from the terms of
sections 15 and 16 of this License; or
:dt.b)
:dd.Requiring preservation of specified reasonable legal notices or author
attributions in that material or in the Appropriate Legal Notices displayed by
works containing it; or
:dt.c)
:dd.Prohibiting misrepresentation of the origin of that material, or requiring
that modified versions of such material be marked in reasonable ways as
different from the original version; or
:dt.d)
:dd.Limiting the use for publicity purposes of names of licensors or authors of
the material; or
:dt.e)
:dd.Declining to grant rights under trademark law for use of some trade names,
trademarks, or service marks; or
:dt.f)
:dd.Requiring indemnification of licensors and authors of that material by
anyone who conveys the material (or modified versions of it) with contractual
assumptions of liability to the recipient, for any liability that these
contractual assumptions directly impose on those licensors and authors.
:edl.

:p.All other non-permissive additional terms are considered "further
restrictions" within the meaning of section 10. If the Program as you received
it, or any part of it, contains a notice stating that it is governed by this
License along with a term that is a further restriction, you may remove that
term. If a license document contains a further restriction but permits
relicensing or conveying under this License, you may add to a covered work
material governed by the terms of that license document, provided that the
further restriction does not survive such relicensing or conveying.

:p.If you add terms to a covered work in accord with this section, you must
place, in the relevant source files, a statement of the additional terms that
apply to those files, or a notice indicating where to find the applicable terms.

:p.Additional terms, permissive or non-permissive, may be stated in the form of
a separately written license, or stated as exceptions; the above requirements
apply either way.

:p.8. Termination.

:p.You may not propagate or modify a covered work except as expressly provided
under this License. Any attempt otherwise to propagate or modify it is void,
and will automatically terminate your rights under this License (including any
patent licenses granted under the third paragraph of section 11).

:p.However, if you cease all violation of this License, then your license from
a particular copyright holder is reinstated (a) provisionally, unless and until
the copyright holder explicitly and finally terminates your license, and (b)
permanently, if the copyright holder fails to notify you of the violation by
some reasonable means prior to 60 days after the cessation.

:p.Moreover, your license from a particular copyright holder is reinstated
permanently if the copyright holder notifies you of the violation by some
reasonable means, this is the first time you have received notice of violation
of this License (for any work) from that copyright holder, and you cure the
violation prior to 30 days after your receipt of the notice.

:p.Termination of your rights under this section does not terminate the
licenses of parties who have received copies or rights from you under this
License. If your rights have been terminated and not permanently reinstated,
you do not qualify to receive new licenses for the same material under section
10.

:p.9. Acceptance Not Required for Having Copies.

:p.You are not required to accept this License in order to receive or run a
copy of the Program. Ancillary propagation of a covered work occurring solely
as a consequence of using peer-to-peer transmission to receive a copy likewise
does not require acceptance. However, nothing other than this License grants
you permission to propagate or modify any covered work. These actions infringe
copyright if you do not accept this License. Therefore, by modifying or
propagating a covered work, you indicate your acceptance of this License to do
so.

:p.10. Automatic Licensing of Downstream Recipients.

:p.Each time you convey a covered work, the recipient automatically receives a
license from the original licensors, to run, modify and propagate that work,
subject to this License. You are not responsible for enforcing compliance by
third parties with this License.

:p.An "entity transaction" is a transaction transferring control of an
organization, or substantially all assets of one, or subdividing an
organization, or merging organizations. If propagation of a covered work
results from an entity transaction, each party to that transaction who receives
a copy of the work also receives whatever licenses to the work the party's
predecessor in interest had or could give under the previous paragraph, plus a
right to possession of the Corresponding Source of the work from the
predecessor in interest, if the predecessor has it or can get it with
reasonable efforts.

:p.You may not impose any further restrictions on the exercise of the rights
granted or affirmed under this License. For example, you may not impose a
license fee, royalty, or other charge for exercise of rights granted under this
License, and you may not initiate litigation (including a cross-claim or
counterclaim in a lawsuit) alleging that any patent claim is infringed by
making, using, selling, offering for sale, or importing the Program or any
portion of it.

:p.11. Patents.

:p.A "contributor" is a copyright holder who authorizes use under this License
of the Program or a work on which the Program is based. The work thus licensed
is called the contributor's "contributor version".

:p.A contributor's "essential patent claims" are all patent claims owned or
controlled by the contributor, whether already acquired or hereafter acquired,
that would be infringed by some manner, permitted by this License, of making,
using, or selling its contributor version, but do not include claims that would
be infringed only as a consequence of further modification of the contributor
version. For purposes of this definition, "control" includes the right to grant
patent sublicenses in a manner consistent with the requirements of this License.

:p.Each contributor grants you a non-exclusive, worldwide, royalty-free patent
license under the contributor's essential patent claims, to make, use, sell,
offer for sale, import and otherwise run, modify and propagate the contents of
its contributor version.

:p.In the following three paragraphs, a "patent license" is any express
agreement or commitment, however denominated, not to enforce a patent (such as
an express permission to practice a patent or covenant not to sue for patent
infringement). To "grant" such a patent license to a party means to make such
an agreement or commitment not to enforce a patent against the party.

:p.If you convey a covered work, knowingly relying on a patent license, and the
Corresponding Source of the work is not available for anyone to copy, free of
charge and under the terms of this License, through a publicly available
network server or other readily accessible means, then you must either (1)
cause the Corresponding Source to be so available, or (2) arrange to deprive
yourself of the benefit of the patent license for this particular work, or (3)
arrange, in a manner consistent with the requirements of this License, to
extend the patent license to downstream recipients. "Knowingly relying" means
you have actual knowledge that, but for the patent license, your conveying the
covered work in a country, or your recipient's use of the covered work in a
country, would infringe one or more identifiable patents in that country that
you have reason to believe are valid.

:p.If, pursuant to or in connection with a single transaction or arrangement,
you convey, or propagate by procuring conveyance of, a covered work, and grant
a patent license to some of the parties receiving the covered work authorizing
them to use, propagate, modify or convey a specific copy of the covered work,
then the patent license you grant is automatically extended to all recipients
of the covered work and works based on it.

:p.A patent license is "discriminatory" if it does not include within the scope
of its coverage, prohibits the exercise of, or is conditioned on the
non-exercise of one or more of the rights that are specifically granted under
this License. You may not convey a covered work if you are a party to an
arrangement with a third party that is in the business of distributing
software, under which you make payment to the third party based on the extent
of your activity of conveying the work, and under which the third party grants,
to any of the parties who would receive the covered work from you, a
discriminatory patent license (a) in connection with copies of the covered work
conveyed by you (or copies made from those copies), or (b) primarily for and in
connection with specific products or compilations that contain the covered
work, unless you entered into that arrangement, or that patent license was
granted, prior to 28 March 2007.

:p.Nothing in this License shall be construed as excluding or limiting any
implied license or other defenses to infringement that may otherwise be
available to you under applicable patent law.

:p.12. No Surrender of Others' Freedom.

:p.If conditions are imposed on you (whether by court order, agreement or
otherwise) that contradict the conditions of this License, they do not excuse
you from the conditions of this License. If you cannot convey a covered work so
as to satisfy simultaneously your obligations under this License and any other
pertinent obligations, then as a consequence you may not convey it at all. For
example, if you agree to terms that obligate you to collect a royalty for
further conveying from those to whom you convey the Program, the only way you
could satisfy both those terms and this License would be to refrain entirely
from conveying the Program.

:p.13. Use with the GNU Affero General Public License.

:p.Notwithstanding any other provision of this License, you have permission to
link or combine any covered work with a work licensed under version 3 of the
GNU Affero General Public License into a single combined work, and to convey
the resulting work. The terms of this License will continue to apply to the
part which is the covered work, but the special requirements of the GNU Affero
General Public License, section 13, concerning interaction through a network
will apply to the combination as such.

:p.14. Revised Versions of this License.

:p.The Free Software Foundation may publish revised and/or new versions of the
GNU General Public License from time to time. Such new versions will be similar
in spirit to the present version, but may differ in detail to address new
problems or concerns.

:p.Each version is given a distinguishing version number. If the Program
specifies that a certain numbered version of the GNU General Public License "or
any later version" applies to it, you have the option of following the terms
and conditions either of that numbered version or of any later version
published by the Free Software Foundation. If the Program does not specify a
version number of the GNU General Public License, you may choose any version
ever published by the Free Software Foundation.

:p.If the Program specifies that a proxy can decide which future versions of
the GNU General Public License can be used, that proxy's public statement of
acceptance of a version permanently authorizes you to choose that version for
the Program.

:p.Later license versions may give you additional or different permissions.
However, no additional obligations are imposed on any author or copyright
holder as a result of your choosing to follow a later version.

:p.15. Disclaimer of Warranty.

:p.THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE
LAW. EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER
PARTIES PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER
EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE ENTIRE RISK AS TO THE
QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU. SHOULD THE PROGRAM PROVE
DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.

:p.16. Limitation of Liability.

:p.IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING WILL
ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MODIFIES AND/OR CONVEYS THE
PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY
GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OR
INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF DATA OR DATA
BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD PARTIES OR A
FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS), EVEN IF SUCH HOLDER
OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

:p.17. Interpretation of Sections 15 and 16.

:p.If the disclaimer of warranty and limitation of liability provided above
cannot be given local legal effect according to their terms, reviewing courts
shall apply local law that most closely approximates an absolute waiver of all
civil liability in connection with the Program, unless a warranty or assumption
of liability accompanies a copy of the Program in return for a fee.
:p.
.ce END OF TERMS AND CONDITIONS


.ce How to Apply These Terms to Your New Programs

:p.If you develop a new program, and you want it to be of the greatest
possible use to the public, the best way to achieve this is to make it
free software which everyone can redistribute and change under these terms.

:p.To do so, attach the following notices to the program.  It is safest
to attach them to the start of each source file to most effectively
state the exclusion of warranty; and each file should have at least
the "copyright" line and a pointer to where the full notice is found.

:xmp.
    &lt.one line to give the program's name and a brief idea of what it does.&gt.
    Copyright (C) &lt.year&gt.  &lt.name of author&gt.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see &lt.http&colon.//www.gnu.org/licenses/&gt..
:exmp.

:p.Also add information on how to contact you by electronic and paper mail.

:p.If the program does terminal interaction, make it output a short
notice like this when it starts in an interactive mode&colon.

:xmp.
    &lt.program&gt.  Copyright (C) &lt.year&gt.  &lt.name of author&gt.
    This program comes with ABSOLUTELY NO WARRANTY; for details type &osq.show w&csq..
    This is free software, and you are welcome to redistribute it
    under certain conditions; type &osq.show c&csq. for details.
:exmp.

:p.The hypothetical commands &osq.show w&csq. and &osq.show c&csq. should show the appropriate
parts of the General Public License.  Of course, your program's commands
might be different; for a GUI interface, you would use an "about box".

:p.You should also get your employer (if you work as a programmer) or school,
if any, to sign a "copyright disclaimer" for the program, if necessary.
For more information on this, and how to apply and follow the GNU GPL, see
&lt.http&colon.//www.gnu.org/licenses/&gt..

:p.The GNU General Public License does not permit incorporating your program
into proprietary programs.  If your program is a subroutine library, you
may consider it more useful to permit linking proprietary applications with
the library.  If this is what you want to do, use the GNU Lesser General
Public License instead of this License.  But first, please read
&lt.http&colon.//www.gnu.org/philosophy/why-not-lgpl.html&gt..

.* htmlsource = intro.htm
:h1 res=3 x=left y=bottom width=100% height=100%.Introduction
.br
Styler&slr.2 is an user interface enhancer for OS&slr.2 Warp 3 and 4&per. Both the
aesthetics and the behavior of various OS&slr.2 Presentation Manager windows
and controls can be easily modified to meet your needs&per.
.br
You will be able to use bitmaps and shades as titlebar backgrounds, choose
any bitmap (of appropriate size) for the window buttons and enable all dialog
windows to use your favorite font&per.
:p.
Other features include&colon.
:ul compact.
.br
:li.window rolling,
:li.titlebar hiding,
:li.overriding the default size and position of maximized windows,
:li.smooth horizontal movement of windows,
:li.snap windows to the desktop edges,
:li.moving a window by dragging its frame,
:li.pushing windows behind the other windows,
:li.selection of the entry field contents,
:li.facilitating clipboard operations via mouse and&slr.or keyboard
combinations in entry fields and command windows,
:li.autofocus,
:li.mouse pointer hiding,
:li.mouse button double-click emulation,
:li.automatically moving the mouse pointer to the focus window&per.
:eul.
.br

.* htmlsource = new.htm
:h1 res=4 x=left y=bottom width=100% height=100%.What&csq.s new
.br
:font facename='Helv' size=16x10.:hp2.
Version 1&per.7&per.5
:font facename=default size=0x0.:ehp2.
:ul compact.
.br
:li.Now it is possible to use bitmaps of any size for the frame
controls (i&per.e&per. maximize, minimize, close buttons etc&per.)&per.
.br
I provided a sample set of frame control bitmaps for XGA or better
resolution (win9x-s&per.bmp) which are 18x22 rather than 22x22 pixels
(with the exception of the system menu which is 24x22 pixels)&per.
.br
The image below shows the difference between standard
and custom sized frame controls&per.
.br
:artwork name='framectls.bmp' align=center.
:hp2.Notes&colon.:ehp2.
:ol compact.
.br
:li.this new feature doesn&csq.t work correctly with windows
based on the Odin library&per. The problem will be corrected in the
near future&per.
:li.the titlebar preview, in the Styler&slr.2 preferences program, cannot
display correctly custom size bitmaps&per.
:eol.
.br
:li.The :hp1.Roll&slr.unroll all windows :ehp1. and
:hp1.Hide&slr.unhide all windows :ehp1. functions, previously
available only via hot key, can now be executed by opening
the corresponding icons created by the Styler&slr.2 installer&per.
.br
The objects icon can be dragged onto the Launchpad&slr.WarpCenter&slr.xCenter&slr.eCenter
to roll&slr.unroll or hide&slr.show all windows with a simple mouse click&per.
:li.Usually the installer asks for confirmation before overwriting
a bitmap file if it is newer then the file being installed&per.
.br
This is useful only if you modified the original bitmaps&per. Overwriting
does not affect new bitmaps you created, with a different name, in
the application subdirectories&per.
.br
To exclude the overwrite prompt open a command window in the path
where you unzipped the distribution package and type&colon.
.br
:font facename='System VIO' size=12x5.
SET STLROVERWRITE=YES
:font facename=default size=0x0.
:hp1.<enter>:ehp1.
.br
then&colon.
.br
:font facename='System VIO' size=12x5.
INSTALL
:font facename=default size=0x0.
:hp1.<enter>:ehp1.
:li.In some particular cases, in Warp 3, the frame controls were
not displayed in the correct order&per.
:li.Frame windows based on the Odin libraries now respect the position
of the close button with the exception of the left position option
(which currently does not work in any kind of window)&per.
:li.Various other minor bugs have been corrected&per.
:eul.
.br
:font facename='Helv' size=16x10.:hp2.
Version 1&per.7&per.3
:font facename=default size=0x0.:ehp2.
:ul compact.
.br
:li.Styler&slr.2 is now distributed in two separated packages&colon. one for
installation on Warp 3 and another for installation on Warp version 4 or
greater (MCP or eCS)&per.
:li.Improved compatibility with Odin based applications (in some cases
Styler&slr.2 enhancements were not correctly applied)&per.
:li.Drop down lists enhancement&colon. press the left mouse button on the
entry field part of a drop down list, then move the mouse down without
releasing the mouse button, the listbox appears and you can
directly select an item by moving the mouse and releasing the button&per.
:li.Documentation of all the Styler&slr.2 programs now is in OS&slr.2 HELP format&per.
:eul.
.br
:font facename='Helv' size=16x10.:hp2.
Version 1&per.7&per.2
:font facename=default size=0x0.:ehp2.
:ul compact.
.br
:li.Mouse motion events were causing SYS3175 exceptions when the option
&cdq.Allow vertical resizing of rolled windows&cdq. (Window rolling page) was
disabled or when the option &cdq.Don&csq.t allow sizing&cdq. (Maximized windows page)
was enabled&per.
:li.In the preferences notebook, the &cdq.Default&cdq. checkbox of the
image selection dialog was not working properly&per.
:li.Titlebar and window controls enhancements (minimize&slr.maximize buttons)
were not applied to all the windows based on the Odin libraries&per.
:li.Redundant and obsolete code has been removed from the procedures of
the frame, titlebar and menu windows, with a small improvement
in speed and resources usage&per.
:li.The &cdq.Snap windows to the screen edges&cdq. feature has been extended to
secondary windows&colon. in a multidocument interface application
(for instance VIEW&per.EXE or Embellish) you can now snap secondary windows
to the edges of the container window&per.
:eul.
.br
:font facename='Helv' size=16x10.:hp2.
Version 1&per.7&per.1
:font facename=default size=0x0.:ehp2.
:ul compact.
.br
:li.Clipboard operations via mouse shortcuts stopped to work
in VIO windows if, while marking, the Ctrl key was released before the
mouse button&per.
:li.Some programs listed in the mouse exceptions crashed with
a SYS3175 exception as soon as the mouse moved on a program window&per.
:li.The preferences program, in some cases, wrongly wrote updated
settings to OS2&per.INI rather than to SMARTWIN&per.INI&per.
:li.In the preferences program, the dialog showing the settings of
the close-window titlebar button, was not displaying the correct
text in the dropdown list used to select the button position&per.
:li.Avoided some system hangs with applications using Odin libraries
(e&per.g&per. innotek java 1&per.4 and innotek adobe acrobat 4)&per.
:li.The preferences program failed to properly save new titlebar
settings when multiple titlebar&slr.titlebar-button images were applied at once&per.
:eul.
.br
:font facename='Helv' size=16x10.:hp2.
Version 1&per.7&per.0
:font facename=default size=0x0.:ehp2.
:ul compact.
.br
:li.Styler&slr.2 has been completely rewritten&per.
:li.Now Styler&slr.2 works correctly with Connectix Virtual PC and hopefully
with other applications based on the Odin libraries&per.
.br
Currently the :hp1.Window rolling :ehp1. and :hp1.Titlebar hiding :ehp1.
features have been disabled for windows relying on those libraries&per.
:li.Most limitations regarding bitmap formats have been removed&per.
.br
Now any kind of OS&slr.2 bitmap is valid provided that its size does not
exceed 64 KBs&per.
:li.Thanks to an undocumented OS&slr.2 API, now Styler&slr.2 shares the same bitmap
resources among all the system processes&per.
.br
This provides a small increment in speed (when starting a new process)
and a large decrease in resource usage (when a lot of processes are
running)&per.
:li.Titlebar gradients use much less memory and are displayed
much faster as they are converted to shared bitmaps and then stretched
as needed&per.
:li.The titlebar sample in the preferences notebook correctly shows
centered text when the :hp1.Center :ehp1. option is selected&per.
:li.When a mouse button is pressed on the window border now the window
correctly receive the focus&per.
:li.Some new bitmaps for the titlebars and the window controls have
been added&per.
:eul.
.br
:p.

.* htmlsource = install.htm
:h1 res=5 x=left y=bottom width=100% height=100%.Installation - uninstallation
.br
The program can be installed over a previous installation of Styler&slr.2 (Smart
Windows) v&per.0&per.81 or higher, without any need to disable the prior version
or to uninstall it&per.
.br
If you installed a version older than v&per.0&per.81 you must delete the
:font facename='System VIO' size=12x5.
SMARTWIN&per.INI
:font facename=default size=0x0.
file from the installation path before installing this version&per.
:p.
The archive smw175&per.zip
.* (or smw174w3 - if you are installing on Warp 3)
must be unzipped in a temporary directory,
taking care to preserve the archive&csq.s path structure&per.
.br
If you are using the Info-ZIP group&csq.s free UnZip extraction utility,
the archive will unzip correctly by default&per. Simply open an OS&slr.2
command window, move to the temporary directory and type&colon.
:xmp.:font facename='System VIO' size=12x5.
UNZIP SMW175&per.ZIP
:exmp.:font facename=default size=0x0.
If you are using a different extraction utility, read its documentation for
the appropriate unzip parameters&per.
:p.
After unzipping the archive, you must run INSTALL&per.EXE and click on the
:hp1.Forward :ehp1.
button to select the action you want to perform&colon.
:ol compact.
.br
:li.Install
:li.Recreate desktop objects
:li.Uninstall
:eol.
.br
:p.
Once you select the proper action, click on the :hp1.Forward :ehp1.
button to install, to recreate Styler&slr.2 desktop objects, or to uninstall&per.
:p.
The other buttons will allow you to&per.&per.&per.
:ul compact.
.br
:li.
go back to the previous page in order to change some previous selection
(for instance, the action to be performed or the installation path)&semi.
:li.
continue with the procedure&semi.
:li.
terminate the program immediately&semi.
:li.
read this documentation&per.
:eul.
.br
:hp2.Note&colon.:ehp2.
If you want to install over a previous version, overwriting all the
files, without being prompted for confirmation on overwriting files
newer than the file being installed (for instance a bitmap file you
modified without assigning it a different name) you have to run the
installer from the command line&colon.
:ol compact.
.br
:li.open a command line window in the path where you unzipped the
distribution package&per.
:li.type&colon.
:font facename='System VIO' size=12x5.
SET STLROVERWRITE=YES
:font facename=default size=0x0.
:hp1.<enter>:ehp1.
:li.type&colon.
:font facename='System VIO' size=12x5.
INSTALL
:font facename=default size=0x0.
:hp1.<enter>:ehp1.
:eol.
.br
:p.
:hp2.Installation details:ehp2.
:p.
Here are the step-by-step details of what happens when you click on the
:hp1.Forward :ehp1. button&per. Please refer to this section if you cannot
properly install the program on your system&per.
:ol.
.br
:li.
The installation program first looks for the flag file
:font facename='System VIO' size=12x5.
SMARTWIN&per.&atsign.
:font facename=default size=0x0.
to enable the :hp1.Install  :ehp1.radio button, then looks in the
:font facename='System VIO' size=12x5.
&bsl.OS2&bsl.DLL
:font facename=default size=0x0.
directory and the
:font facename='System VIO' size=12x5.
OS2&per.INI
:font facename=default size=0x0.
file for any item from a previous installation&per.
.br
If any trace of a previous installation is found (or if you run the
:hp1.Installer&slr.Uninstaller :ehp1.
object later) the :hp1.Recreate desktop objects :ehp1. and
:hp1.Uninstall :ehp1.
radio buttons will be enabled as well&per.
:li.
If you select to install, the next page will show you the :hp1.License agreement:ehp1.
&per. Please read it carefully before continuing with the installation&per.
:li.
On the next page, you can choose the installation path&per. The default drive
is your boot drive and the default path is
:font facename='System VIO' size=12x5.
&bsl.SMARTWIN
:font facename=default size=0x0.
unless you
are upgrading a previous installation, in which case the previous installation
path will be the default&per.
:p.
Note that if you ever choose to uninstall Styler&slr.2, the program files
will be deleted and then a message box will ask your permission to
remove the entire installation path with all of its contents&per.
Therefore, it is advisable to avoid installing the program files in a
directory which will contain other files or folders not related to
Styler&slr.2&per.
:li.
Click on :hp1.Forward :ehp1. to create the selected path&per. If the path doesn&csq.t
exist, the program will create all the necessary directories&per. For instance,
if you selected to install the program files in
:font facename='System VIO' size=12x5.
C&colon.&bsl.APPS&bsl.TEST&bsl.SMW
:font facename=default size=0x0.
and none of the directories in the path exist, then the installation program
will create all of them&per.
.br
If, for any reason, it is not possible to create the selected path,
you should click on :hp1.Back:ehp1.  and select a different path&per.
:li.
The final panel allows you to modify your previous
selections&per.
:p.
Click on :hp1.Forward :ehp1. to start installing the files&per.
:li.
A progress dialog will allow you to check the state of the installation&per.
:p.
The installation procedure consists of the following steps&colon.
:ul compact.
.br
:li.
If the :hp1.Preferences:ehp1.  program is open, it will be closed&per.
:li.
STYLER20 and SMARTWIN are added to the OS2&per.INI list of DLLs to be loaded
every time a new process is started (:hp1.SYS_DLLS :ehp1. - :hp1.LoadPerProcess:ehp1.)&per.
:li.
The installation path is stored in OS2&per.INI&per.
:li.
If the program MPTS&per.EXE is present in one of the directories mentioned
in the path statement of the CONFIG&per.SYS file, it is added to the
:link reftype=hd res=8.exceptions:elink.
in order to let it work properly with Styler&slr.2&per.
:li.
All unneeded files from a previous installation are deleted&per.
:li.
The following files needed for the current installation are copied to the
selected path&colon.
:ul compact.
.br
:li.
:font facename='System VIO' size=12x5.
AFCUTL05&per.DLL
:font facename=default size=0x0.
:li.
:font facename='System VIO' size=12x5.
INSTALL&per.EXE
:font facename=default size=0x0.
:li.
:font facename='System VIO' size=12x5.
LICENSE&per.TXT
:font facename=default size=0x0.
:li.
:font facename='System VIO' size=12x5.
SMARTWIN&per.EXE
:font facename=default size=0x0.
:li.
:font facename='System VIO' size=12x5.
STLRES13&per.DLL
:font facename=default size=0x0.
:li.
:font facename='System VIO' size=12x5.
STYLER2&per.HLP
:font facename=default size=0x0.
:li.
:font facename='System VIO' size=12x5.
README&per.TXT
:font facename=default size=0x0.
:li.
:font facename='System VIO' size=12x5.
RUAW&per.ICO
:font facename=default size=0x0.
:li.
:font facename='System VIO' size=12x5.
HSAW&per.ICO
:font facename=default size=0x0.
:eul.
.br
:li.
The file SMARTWIN&per.170 is copied as SMARTWIN&per.DLL in &bsl.OS2&bsl.DLL&per.
:li.
The file STYLER20&per.170 is copied as STYLER20&per.DLL in &bsl.OS2&bsl.DLL&per.
:li.
All the bitmap files used for the titlebar and the window controls are
copied to the installation directory&per. If you have modified a bitmap, the
program will ask for your confirmation to overwrite it&per.
:li.
A Styler&slr.2 folder object is created on the desktop,
with the following items inside&colon.
:ul compact.
.br
:li.
User&csq.s Guide -- the program documentation in HLP format&semi.
:li.
Read Me -- an HTML file with some notes on the program&semi.
:li.
Preferences -- the preferences program&semi.
:li.
License Agreement -- a text file&semi.
:li.
Installer&slr.Uninstaller -- the installation&slr.uninstallation program&per.
:li.
Hide&slr.show all windows -- a program object to hide&slr.show all the desktop windows&per.
:li.
Roll&slr.unroll all windows -- a program object to roll&slr.unroll all the desktop windows&per.
:eul.
.br
:li.
A shadow of the Preferences Notebook is created in the OS&slr.2 System Setup folder&per.
:eul.
.br
:li.
At the end of the installation procedure, a message will indicate if the installation
has been successful, or if any critical or non-critical error has occurred&per.
:li.
On the last page, the log file will show the details of the installation&per.
:eol.
.br
:p.
:hp2.Uninstallation details:ehp2.
:p.
The uninstallation procedure reverses all of the operations described above&per.
Besides that, it will delete any new bitmap files you added to the
:font facename='System VIO' size=12x5.
&bsl.BITMAPS
:font facename=default size=0x0.
subdirectories&per. In any case, a message box will ask you to confirm the deletion of
the installation path and other files which still can be there (such as
:font facename='System VIO' size=12x5.
SMARTWIN&per.INI
:font facename=default size=0x0.
created by the application, plus any other files that you yourself put there)&per.
:p.
At the end of the uninstallation procedure, the
:font facename='System VIO' size=12x5.
SMWSETUP&per.LOG
:font facename=default size=0x0.
file will be written in the root of the boot drive&per.

.* htmlsource = features.htm
:h1 res=6 x=left y=bottom width=100% height=100%.Features and settings
.br
The program consists of a DLL loaded by the system every time a
new process is started, and a Preferences Notebook which runs Styler&slr.2
according to your favorite customized settings&per.
.br
The Notebook pages share a common area with a five-button tool bar and
a status bar providing contextual help&per. If you cannot read a help
message because the text doesn&csq.t fit inside the status bar, you can
enlarge the Preferences window-- it is resizeable!
:p.
The toolbar buttons have the following functions&colon.
:p.
:sl compact.
:li.:artwork name='styler03.bmp' align=left runin.
The first button :hp2.enables&slr.disables:ehp2. all Styler&slr.2 features&per.
:p.
:artwork name='styler04.bmp' align=left runin.
This button is required to :hp2.apply:ehp2. the settings of the current page&per.
Most pages do not need it since the settings become effective as soon as they
are changed&per. When required, the button is enabled as soon as a setting
is modified&per.
:p.
:artwork name='styler05.bmp' align=left runin.
The third button is an :hp2.undo:ehp2. button&per. It will set the settings of
the current page to the values they held when the Preferences Window
was opened&per.
:p.
:artwork name='styler06.bmp' align=left runin.
The fourth button is the :hp2.default:ehp2. button and it resets the current
page preferences to the default values&per.
:p.
:artwork name='styler07.bmp' align=left runin.
The fifth button is the help button&per. By clicking on it, you will get the
Styler&slr.2 User&csq.s Guide page corresponding to the current settings page&per.:esl.
There are still some features not yet implemented though the corresponding
buttons&slr.checkboxes appear in the Preferences application&per.
:p.
The Styler&slr.2 Preferences Notebook consists of the following pages&colon.
:ul.
.br
:li.
:hp2.Styler&slr.2:ehp2.
:ul compact.
.br
:li.
Product information (just the logo and the version number)
:li.
:link reftype=hd res=7.General:elink.
:li.
:link reftype=hd res=8.Exceptions:elink.
:eul.
.br
:li.
:hp2.Dialogs:ehp2.
:ul compact.
.br
:li.
:link reftype=hd res=9.General options:elink.
:li.
:link reftype=hd res=8.Exceptions:elink.
:eul.
.br
:li.
:hp2.Titlebar:ehp2.
:ul compact.
.br
:li.
:link reftype=hd res=10.Active window:elink.
:li.
:link reftype=hd res=10.Inactive window:elink.
:li.
:link reftype=hd res=12.Titlebar hiding:elink.
:li.
:link reftype=hd res=8.Exceptions:elink.
:eul.
.br
:li.
:hp2.Window rolling:ehp2.
:ul compact.
.br
:li.
:link reftype=hd res=13.General options:elink.
:li.
:link reftype=hd res=8.Exceptions:elink.
:eul.
.br
:li.
:hp2.Maximized windows:ehp2.
:ul compact.
.br
:li.
:link reftype=hd res=14.General options:elink.
:li.
:link reftype=hd res=8.Exceptions:elink.
:eul.
.br
:li.
:hp2.Sizing&slr.moving:ehp2.
:ul compact.
.br
:li.
:link reftype=hd res=15.General options:elink.
:li.
:link reftype=hd res=8.Exceptions:elink.
:eul.
.br
:li.
:hp2.Text controls:ehp2.
:ul compact.
.br
:li.
:link reftype=hd res=16.General options:elink.
:li.
:link reftype=hd res=8.Exceptions:elink.
:eul.
.br
:li.
:hp2.VIO windows:ehp2.
:ul compact.
.br
:li.
:link reftype=hd res=17.General options:elink.
:li.
:link reftype=hd res=8.Exceptions:elink.
:eul.
.br
:li.
:hp2.Mouse:ehp2.
:ul compact.
.br
:li.
:link reftype=hd res=18.General options:elink.
:li.
:link reftype=hd res=8.Exceptions:elink.
:li.
:link reftype=hd res=19.Mapping:elink.
:eul.
.br
:li.
:hp2.Keyboard:ehp2.
:ul compact.
.br
:li.
:link reftype=hd res=19.Mapping:elink.
:li.
:link reftype=hd res=20.Windows keys:elink.
:eul.
.br
:eul.
.br
:hp2.Other dialogs:ehp2. used by the application are&colon.
:ul compact.
.br
:li.
The
:link reftype=hd res=11.bitmap selection dialog:elink.
:li.
The
:link reftype=hd res=19.mouse&slr.keyboard functions dialog:elink.
:eul.
.br

.* htmlsource = pggener.htm
:h2 res=7 x=left y=bottom width=100% height=100%.Styler&slr.2 - General page
.br
The settings in this page affect the behaviour of the Preferences
Notebook itself&colon.
:dl break=all.
.br
:dt.
:hp1.Apply on exit:ehp1.
:dd.
All new settings will be automatically
:link reftype=hd res=6.applied:elink.
when the program window is closed&per.
:dt.
:hp1.Save on exit:ehp1.
:dd.
If not checked, the current settings are not saved in SMARTWIN&per.INI, so you will
get back the previous settings when you reboot&per.
:dt.
:hp1.Remember last viewed page:ehp1.
:dd.
This option works when the program is started and brings up the page
which was topmost when the program was closed&per.
:dt.
:hp1.Apply all:ehp1.
:dd.
The first button simulates clicking on the
:hp1.
:link reftype=hd res=6.Apply:elink.
 :ehp1. toolbar icon
on all pages (which require that) to make the current preferences
the active ones&per.
:dt.
:hp1.Undo all:ehp1.
:dd.
The middle button simulates clicking on the
:hp1.
:link reftype=hd res=6.Undo:elink.
 :ehp1. toolbar icon on all pages
where some settings have been modified&per.
:dt.
:hp1.Default all:ehp1.
:dd.
The third button simulates clicking on the
:hp1.
:link reftype=hd res=6.Default :elink.
:ehp1. toolbar icon
in all pages&per.
:edl.
.br
:p.

.* htmlsource = pgexcept.htm
:h2 res=8 x=left y=bottom width=100% height=100%.Styler&slr.2 - Exceptions page
.br
Although Styler&slr.2 integrates seamlessly with the system, generally
with no problem for all the other programs, some applications still cannot properly
cooperate with it and won&csq.t work properly when some Styler&slr.2 features have been set&per.
.br
In order to avoid these kinds of problems, each Preferences page with a given
category of features is followed by an :hp1.Exceptions :ehp1. page allowing
you to enter the pathnames of the programs for which that particular group
of options causes conflicts&per.
.br
In some cases, you would find it more comfortable just to completely disable
Styler&slr.2 enhancements for a given program&per. For this purpose, you should
use the Exception page inserted under the :hp1.Styler&slr.2 :ehp1. main tab&per.
.br
To add a program to the exception list, either type its full pathname (i&per.e&per. its
name including the program path) in the entry field and click on the :hp1.Add :ehp1.
button or get it from the file dialog by clicking on the :hp1.Find :ehp1.
button&per. The :hp1.Remove :ehp1. button will delete a file name from the
list&per.
:p.
:hp2.Note&colon.:ehp2.
:p.
Currently it is not possible to specify the names of programs
whose windows are executed as a PMSHELL process&per. These include all text mode
programs (DOS and OS&slr.2), all Win-OS&slr.2 programs, all folders and all WPS
applications (like the System clock, the color and font palettes, etc&per.)&per.
:p.
In the next version, all these scattered Exceptions pages will be grouped
in a unique dialog, enabling you to add program names by drag &amp. drop
or directly by the system menu of a running application, and also to detect
the various windows belonging to the PMSHELL process&per.

.* htmlsource = pgdialog.htm
:h2 res=9 x=left y=bottom width=100% height=100%.Dialogs - General options page
.br
Usually, when no other kind of font has been explicitly specified
by the programmer, all dialog windows will use the ugly :hp1.System Proportional :ehp1.
font&per.
:p.
Styler&slr.2 allows you to override this system behaviour and force all dialog
windows to use your favourite font&per. Just check :hp1.Use Styler&slr.2 font :ehp1.
to enable this function&per.
.br
This will work in all dialog windows which use the system default font&per.
If you want to force all windows to use the font you selected, then check
:hp1.Override
Presentation Parameters :ehp1.&per.
:p.
The :hp1.Font :ehp1. button brings the :hp1.Dialog font :ehp1.
dialog to the top&per.
:p.
:hp2.Note&colon.:ehp2.
:p.
dialog controls inherit the font attribute (presentation
parameter) from the dialog unless they have been explicitly set to use
a different font&per. In this latter case, the :hp1.Use Styler&slr.2 font :ehp1.
feature won&csq.t work&per.

.* htmlsource = pgtbar.htm
:h2 res=10 x=left y=bottom width=100% height=100%.Titlebar - Active &slr. Inactive window pages
.br
Through this page it is possible to change the look of titlebars
completely&per.
:p.
You can use any bitmap for the titlebar background and the window controls
provided that they are proper OS&slr.2 bitmaps and smaller than 64 KBs&per.
:p.
The settings in this page must be applied through the :hp1.
:link reftype=hd res=6.Apply:elink.
 :ehp1.
button to become active&per.
:dl break=all.
.br
:dt.
:hp1.Enable titlebar enhancements:ehp1.
:dd.
It enables&slr.disables all the controls in the page and the corresponding settings&per.
When it is checked, a titlebar sample will be displayed&per.
:dt.
:hp1.Context menu:ehp1.
:dd.
A context menu event (right or left button click according to what you
set in the system mouse preferences) on the titlebar will make the system
menu pop up next to the mouse pointer&per.
:dt.
:hp1.Titlebar sample:ehp1.
:dd.
It is interactive! By clicking on the titlebar buttons you can modify the
default button bitmaps or the close button position&per. By clicking on the
titlebar you&csq.ll be able to select a different background color, gradient
direction or bitmap according to the selected background mode&per.
:edl.
.br
:hp2.Background group&colon.:ehp2.
:dl break=all.
.br
:dt.
:hp1.Solid color:ehp1.
:dd.
This replicates the default titlebar style&per. The Background button allows you to
select the background color&per. The color selection button can be used like
the titlebar sample&per.
:dt.
:hp1.Gradient:ehp1.
:dd.
It allows the use of a gradient as a titlebar background&per. It will display
four buttons to select the two different colors, to swap them or change the
gradient direction&per. The gradient direction button can be used like the titlebar
sample&per.
:p.
The direction button will allow you to cycle through the available gradient
styles&colon. horizontal, vertical, horizontal-center and vertical-center&per.
:dt.
:hp1.Bitmap:ehp1.
:dd.
Use a bitmap as titlebar background&per. When selected, a bitmap
selection button will be displayed&per. The bitmap selection button can be
used like the titlebar sample&per.
.br
The bitmap is tiled by default&per. If the :hp1.Stretch :ehp1. checkbox
is checked it will be stretched to fit the titlebar size&per.
:dt.
:hp1.Override Presentation Parameters:ehp1.
:dd.
When unchecked, titlebars which do not use default colors for the active
or inactive state won&csq.t use the Styler&slr.2 titlebar background settings&per.
:edl.
.br
:dl break=all.
.br
:dt.
:hp2.3D border group&colon.:ehp2.
:dd.
.br
Use this setting to surround the titlebar with a 3D border&per. The buttons
allow you to select and swap the colors&per.
:edl.
.br
:hp2.Text style group&colon.:ehp2.
:dl break=all.
.br
:dt.
:hp1.Center:ehp1.
:dd.
When checked, the text is centered in the titlebar rather than left-aligned&per.
:dt.
:hp1.3D text:ehp1.
:dd.
When checked, this gives a 3D effect to the text&per. By selecting the colors, you
can get a raised or depressed effect&per.
:dt.
:hp1.Font:ehp1.
:dd.
The :hp1.Font:ehp1.  button allows you to set the titlebar font&per. The same effect
can be attained by dragging a font from the font palette and dropping it
on the titlebar sample&per.
:edl.
.br
:hp2.Note&colon.:ehp2.
:ol.
.br
:li.
On titlebar buttons, the same bitmap is used both for the button in ordinary
state and for the button in depressed state&per. The bitmap is shifted one
pixel left and down for user feedback when the button is depressed&per. In
a future version I&csq.ll try to use icons, i&per.e&per. transparent bitmaps, for extending
the titlebar background along the bottom of the buttons, and to use different images for
the different states of the buttons&per.
:li.
When moved to the left side, the close button replicates the system menu
icon if present&per. The problem will be solved in a future version&per.
:li.
The next version will allow more control of the titlebar settings&colon.
:ul compact.
.br
:li.
3-color gradients, with some dithering and choice of any direction (even
oblique)
:li.
pattern overlay for titlebar background
:li.
text formatting (left, center, right)
:li.
opaque text background etc&per.
:eul.
.br
:eol.
.br

.* htmlsource = pgbmpdlg.htm
:h2 res=11 x=left y=bottom width=100% height=100%.The bitmap selection dialog
.br
This dialog will pop up when you click on the titlebar sample buttons,
on the titlebar background bitmap button or on the buttons in the :hp1.Titlebar
hiding :ehp1. and :hp1.Window rolling :ehp1. pages&per.
.br
This dialog is resizeable and its last size and position are saved in
SMARTWIN&per.INI&per.
:p.
The dialogs used to select the bitmaps for the close button, window
roll&slr.unroll and titlebar hide&slr.unhide have the :hp1.Disable :ehp1. check
box which allows you to remove the button from the titlebar controls&per.
.br
Besides that, the :hp1.Close button bitmap :ehp1. dialog has a combo box
for selecting the button position&per.
:p.
The :hp1.Edit :ehp1. button opens the Icon editor with the bitmap currently
selected&per.
:p.
The :hp1.Import&per.&per.&per. :ehp1. button opens a file dialog for copying the
selected bitmap file to the appropriate path and then refreshes the bitmap list box
contents&per.
:p.
The :hp1.OK :ehp1. and :hp1.Cancel :ehp1. buttons have the usual functions,
while the Help  button will open the appropriate page of
the
:hp1.Styler&slr.2 User&csq.s Guide :ehp1.&per.
:p.
:hp2.Note&colon.:ehp2.
:p.
Currently it is necessary to dismiss the dialog in order to update
the titlebar sample&per. In Styler&slr.2 version 2, the bitmap sample will be
updated as soon as a new bitmap is selected&per.

.* htmlsource = pghtbar.htm
:h2 res=12 x=left y=bottom width=100% height=100%.Titlebar - Titlebar hiding page
.br
The options in this page allow you to hide the titlebar and controls
(optionally including the actionbar menu) of any window&per. The titlebar pops
up again as soon as the mouse pointer moves on the upper window border
and is hidden when the mouse pointer is moved downwards and away from the
titlebar area&per.
:dl break=all.
.br
:dt.
:hp1.Enable titlebar hiding:ehp1.
:dd.
Enable&slr.disable all the titlebar hiding options&per.
:dt.
:hp1.Hide by&colon.:ehp1.
:dl compact break=all.
.br
:dt.
:hp1.System menu:ehp1.
:dd.
Add a &cdq.Titlebar hiding&cdq. item to the window system menu&per.
:dt.
:hp1.Mouse:ehp1.
:dd.
Select which mouse event, performed on the titlebar, toggles titlebar hiding
on&slr.off&per.
:dt.
:hp1.Keyboard:ehp1.
:dd.
Set a keyboard accelerator for toggling titlebar hiding on&slr.off&per.
:dt.
:hp1.Titlebar Hide&slr.Unhide button:ehp1.
:dd.
Use a titlebar button for toggling titlebar hiding on&slr.off&per. Two
different bitmaps are used for hiding&slr.unhiding&per.
:edl.
.br
:dt.
:hp1.Reduce folders height:ehp1.
:dd.
When hiding the titlebar of folders, the total window height is reduced
by the titlebar height&per.
:dt.
:hp1.Exclude dialog windows:ehp1.
:dd.
Disable titlebar hiding for dialog windows&per. Dialog windows without a sizing
border are treated as OS&slr.2-DOS command windows&colon. the titlebar is removed
by reducing the size of the window&per.
:dt.
:hp1.Include menu:ehp1.
:dd.
Hide the actionbar menu alongside the titlebar and window controls&per.
:dt.
:hp1.Pop up delay:ehp1.
:dd.
Sets how long (hundredths of a second) the mouse must stay on the upper window
border before the titlebar is displayed&per.
:dt.
:hp1.Hide delay:ehp1.
:dd.
Sets how long the mouse pointer must stay away from the titlebar area before
the titlebar is hidden again&per.
:edl.
.br
:p.
:hp2.Note&colon.:ehp2.
:p.
Titlebars in hidden state are hidden again only when the mouse pointer
moves downwards inside the window!
.br
There is no reason to hide the titlebar of rolled windows so you get
just a system warning sound if you try to do so&per.

.* htmlsource = pgwroll.htm
:h2 res=13 x=left y=bottom width=100% height=100%.Window rolling - General options page
:dl break=all.
.br
:dt.
:hp1.Enable window rolling:ehp1.
:dd.
Enable&slr.disable all the window rolling options&per.
:dt.
:hp1.Roll by&colon.:ehp1.
:dl compact break=all.
.br
:dt.
:hp1.System menu:ehp1.
:dd.
Add a &cdq.Roll&slr.Unroll&cdq. item to the window system menu&per.
:dt.
:hp1.Mouse:ehp1.
:dd.
Select which mouse event, performed on the titlebar, toggles window rolling
on&slr.off&per.
:dt.
:hp1.Keyboard:ehp1.
:dd.
Set a keyboard accelerator for toggling
window rolling on&slr.off&per.
:dt.
:hp1.Titlebar Roll&slr.Unroll button:ehp1.
:dd.
Use a titlebar button for toggling window rolling on&slr.off&per. Two different
bitmaps are used for rolling&slr.unrolling&per.
:edl.
.br
:dt.
:hp1.Auto-unroll when getting focus:ehp1.
:dd.
Rolled windows are automatically unrolled when getting focus&per.
:dt.
:hp1.Auto-roll when losing focus:ehp1.
:dd.
Windows (if their rolled state has been previously set) are rolled up as they
lose focus&per.
:dt.
:hp1.Allow vertical resizing of rolled windows:ehp1.
:dd.
Disables window resizing for windows in rolled state&per.
:dt.
:hp1.Auto-unroll windows vertically resized:ehp1.
:dd.
Rolled windows are automatically unrolled if resized along the y-axis&per.
:dt.
:hp1.Roll to the bottom:ehp1.
:dd.
The window will roll downward, i&per.e&per., the left bottom corner of the window
won&csq.t move&per.
:dt.
:hp1.Auto-unroll when closing window:ehp1.
:dd.
Rolled windows are unrolled when they are closed or when the system is
closed through the default shutdown dialog&per.
:edl.
.br
:p.
:hp2.Note&colon.:ehp2.
:p.
The :hp1.Auto-roll when losing focus :ehp1. feature is a counterpart of
:hp1.Auto unroll when getting focus :ehp1.&per. When both features are enabled, the
first roll command (through the titlebar, system menu or Titlebar Roll
button) sets the window in an :hp2.:hp1.auto-roll state:ehp1.:ehp2.&colon. windows in
this state are rolled&slr.unrolled as they get&slr.lose focus&per.
:p.
Until the roll state is active, the titlebar button will display the Unroll
bitmap even if the window is currently unrolled (because it is the focus
window)&per.
:p.
If a window in auto-roll state has the focus, it is automatically
unrolled&per. At this point, if you want to roll it again while it is still
the focus window (this is likely to happen only when you are dealing with
maximized windows), two consecutive toggle-roll-state commands are required&colon.
the first will reset the auto-roll state, and the second command will
actually roll the window&per.
:p.
In order to avoid such repetition, you can keep the
Shift key pressed while issuing the roll command&per.

.* htmlsource = pgmaxwin.htm
:h2 res=14 x=left y=bottom width=100% height=100%.Maximized windows - General options page
.br
These settings allow you to change the behaviour of maximized windows&per.
:dl break=all.
.br
:dt.
:hp1.Override default size and position:ehp1.
:dd.
This option allows you to set the position and size of windows when they are
maximized&per. It doesn&csq.t work for OS&slr.2 and DOS command windows or other windows
whose maximized dimension is already smaller than the default size&per.
.br
The window position can be a negative number&per.
:dt.
:hp1.Don&csq.t allow moving:ehp1.
:dd.
This option prevents maximized windows from being moved from their default position&per.
:dt.
:hp1.Don&csq.t allow sizing:ehp1.
:dd.
This options prevents maximized windows from being resized&per.
:edl.
.br

.* htmlsource = pgmotion.htm
:h2 res=15 x=left y=bottom width=100% height=100%.Window moving - General options page
.br
These options control how windows are moved&per.
:dl break=all.
.br
:dt.
:hp1.Full window drag and size:ehp1.
:dd.
Repaint the window synchronously as it is
resized or moved on the screen&per.
:dt.
:hp1.Smooth horizontal movements:ehp1.
:dd.
Move the windows horizontally by 1-pixel steps instead
of the default 8-pixel steps&per.
.br
This feature doesn&csq.t work with OS&slr.2 and DOS command windows&per.
:dt.
:hp1.Snap windows to the screen edges :ehp1.
:dd.
Align the window along the screen edges&per.
.br
Clicking on the upper left corner in the titlebar will immediately move
the window to the upper left corner of the screen&per. Clicking
on the titlebar center will immediately center the window in the screen,
and so on&per.
.br
The :hp1.Snap by&colon. :ehp1. button sets
your favourite mouse &amp. keyboard combination to perform the operation&per.
:dt.
:hp1.Frame drag:ehp1.
:dd.
Drag windows by placing the mouse pointer on the window borders and
dragging them while holding down the mouse button&per. Optionally, set the
:hp1.Drag by&colon. :ehp1. button to use the mouse button and keys that suit
your needs&per.
:dt.
:hp1.Push behind all other windows:ehp1.
:dd.
Move windows along the z-axis, i&per.e&per. move the topmost
windows behind all other windows&per.
.br
The :hp1.Push one step behind :ehp1. sub-option allows you to push the window step
by step behind the other windows&per.
.br
The :hp1.Push by&colon. :ehp1. button allows you to select the mouse event
plus any optional key in order to push the window behind the other windows&per.
:edl.
.br
:p.
:hp2.Note&colon.:ehp2.
:p.
There are still a few minor problems related to the
:hp1.Full window drag &amp. size :ehp1. and the
:hp1.Frame drag :ehp1. features&colon.
:ul compact.
.br
:li.When moving OS&slr.2 and DOS command windows via the keyboard,
the mouse pointer doesn&csq.t follow the window movements&per.
:li.When the middle mouse button is selected for performing a
:hp1.Frame drag :ehp1. yet :hp1.Full window drag &amp. size :ehp1.
is not enabled, it will be necessary to click with the left or right mouse
button to terminate the drag operation&per.
:eul.
.br
These problems are related to system limitations and since they are not
likely to cause any real trouble, no fix is planned&per.

.* htmlsource = pgtext.htm
:h2 res=16 x=left y=bottom width=100% height=100%.Text controls - General options page
.br
These options enhance some text controls&colon.
:dl break=all.
.br
:dt.
:hp1.Entry field automatic text selection:ehp1.
:dd.
This option allows you to select the entire contents of any entry field&per.
.br
The :hp1.Select by&colon. :ehp1. button sets your favourite
mouse and keyboard combination to perform the operation&per.
:dt.
:hp1.Mouse copy&slr.cut&slr.paste:ehp1.
:dd.
This option enables the mouse to perform clipboard operations in various text controls&per.
.br
Only copy operations are allowed in non-editable controls such as :hp1.static
text:ehp1. , :hp1.buttons :ehp1.and :hp1.list boxes :ehp1.&per.
:edl.
.br
:p.
:hp2.Note&colon.:ehp2.
:ul compact.
.br
:li.
Only the selected items of a list box are copied to the clipboard&per.
:li.
The :hp1.Paste (insert) :ehp1. feature is redundant and will be removed
in the next version&per.
:li.
In order to avoid any conflict with events used by the system, carefully
select the combination of mouse buttons and keys that will perform a
clipboard operation&per.
.br
For instance, if the extensions are to be applied to list boxes, then the
combination of Ctrl + Mouse Button 1 for copying text to the clipboard
will conflict with item selection in :hp1.extended selection  :ehp1.list
boxes&per.
:eul.
.br

.* htmlsource = pgviowin.htm
:h2 res=17 x=left y=bottom width=100% height=100%.VIO windows - General options page
.br
These options allow you to enhance all DOS and OS&slr.2 command windows&per.
:dl break=all.
.br
:dt.
:hp1.Eliminate horizontal scrollbars:ehp1.
:dd.
When this feature is active, all command windows resized by a MODE command
are automatically widened to their maximum size and the horizontal scrollbar
is removed&per.
:dt.
:hp1.Make insert mode the default:ehp1.
:dd.
The command window&csq.s keyboard input is always in :hp1.overwrite :ehp1.
mode unless you press the :hp1.Insert :ehp1. key, and input normally will
revert to :hp1.overwrite :ehp1. mode as soon as you press :hp1.Enter :ehp1.&per.
.br
This feature allows you to work always in :hp1.insert :ehp1. mode, even
after you have pressed :hp1.Enter :ehp1.&per.
.br
To use this feature once the option has been activated, you must press
:hp1.Alt
+ Ins :ehp1. to toggle :hp1.insert :ehp1. mode On&slr.Off&per.
:dt.
:hp1.Enable mouse copy&slr.paste:ehp1.
:dd.
This option lets you use the mouse to copy and paste text in command
windows&per.
.br
In Warp 4 you can disable the system :hp1.VIO Mouse Actions :ehp1.
feature and instead use your favourite combinations of mouse and keyboard
to copy and paste text from the clipboard&per.
:dl break=all.
.br
:dt.
:hp1.Copy by&colon.:ehp1.
:dd.
Text marking starts as soon as the selected mouse button is pressed down,
and the marked text is copied to the clipboard when the mouse button is released&per.
.br
You can choose the mouse button and the optional control keys which
will be pressed to perform the clipboard copy&per.
:dt.
:hp1.Copy all by&colon.:ehp1.
:dd.
The selected mouse button click, plus optional control keys, allow you
to copy the entire visible contents of the command window to the clipboard&per.
:dt.
:hp1.Paste by&colon.:ehp1.
:dd.
Pressing a mouse button plus optional control keys will paste the text at
the cursor position&per.
:edl.
.br
.br
:hp2.Note&colon.:ehp2.
.br
The system :hp1.VIO Mouse Actions :ehp1. feature may be enabled or
disabled from the :hp1.User Interface :ehp1. page of the :hp1.System  :ehp1. object
in the :hp1.System Setup :ehp1. folder&per.
.br
The buttons grouped below this option allow you to select the mouse event
and control keys which perform the clipboard operations&per.
:dt.
:hp1.Enable keyboard copy&slr.paste:ehp1.
:dd.
Use the keyboard to copy and paste text in command windows, as you are
accustomed to doing in ordinary text windows&per.
:dl break=all.
.br
:dt.
:hp1.Copy by&colon.:ehp1.
:dd.
The selected combination of keys will switch the command window to marking
mode&per. You have to use the arrow keys to move the block cursor to the point
from which you want to start text selection&per.
.br
Then press the :hp1.Shift :ehp1. key and hold it down while still using the arrow
keys to define the selection area&per. As soon as you have selected all the
text, release the :hp1.Shift :ehp1. key to copy the selected text to the clipboard&per.
:dt.
:hp1.Copy all by&colon.:ehp1.
:dd.
The selected combination of keys will copy the entire visible contents of
the command window to the clipboard&per.
:dt.
:hp1.Paste by&colon.:ehp1.
:dd.
The selected combination of keys will paste the clipboard content, if it
is valid text data, at the current cursor position inside the command window&per.
:edl.
.br
:edl.
.br

.* htmlsource = pgmouse.htm
:h2 res=18 x=left y=bottom width=100% height=100%.Mouse - General options page
.br
The options of this page are related to the mouse events&per.
:dl break=all.
.br
:dt.
:hp1.Automatic focus:ehp1.
:dd.
The window (desktop child) under the mouse pointer is automatically made
active&per. The :hp1.Delay :ehp1. spin box allows you to set how long the mouse
must stay on a window before it becomes the active window&per. This option
works for seamless Win-OS2 windows as well&per.
:dl break=all.
.br
:dt.
:hp1.Ignore desktop:ehp1.
:dd.
The desktop window is not made active when the mouse pointer moves on it&per.
:dt.
:hp1.Don&csq.t bring to top:ehp1.
:dd.
The activated window is not made the topmost window&per. This feature is disabled
when moving to or from a seamless Win-OS2 window&per.
:dt.
:hp1.Ignore new windows:ehp1.
:dd.
The autofocus feature is suspended when a new window is opened, and is re-enabled
only after the mouse pointer has been moved to the new window&per.
:edl.
.br
:dt.
:hp1.Pointer hiding:ehp1.
:dd.
The pointer is hidden when it stops moving&per. The :hp1.Delay:ehp1. spin box allows
you to set how long it must stay still before it is hidden&per.
:dl break=all.
.br
:dt.
:hp1.Exclude buttons:ehp1.
:dd.
Disables pointer hiding when it is over a button&per.
:dt.
:hp1.Exclude menu:ehp1.
:dd.
Disables pointer hiding when it is over a menu&per.
:edl.
.br
:dt.
:hp1.Double click emulation:ehp1.
:dd.
Works with most PM windows and applications but still works randomly with
seamless Win-OS2 windows&per.
.br
The :hp1.Include titlebar :ehp1. sub-option extends the emulation
on titlebars&per.
.br
By default the double click emulation doesn&csq.t work on titlebars since
middle mouse button clicks are used for titlebar hiding and for window rolling&per.
By activating this sub-option, a middle mouse button click on the titlebar
will maximize&slr.restore the window&per.
:dt.
:hp1.Move the mouse pointer to the focus window:ehp1.
:dd.
This group of features allows you to automatically move the mouse pointer to
the window which gets the focus&per.
.br
When none of the sub-options is checked, the feature works with all windows&per.
.br
When :hp1.New windows :ehp1. and&slr.or :hp1.Dialogs :ehp1. are checked, the
feature works when a new window is created or when a dialog gets the focus&per.
.br
:hp1.Exclude buttonless dialogs :ehp1. lets Styler&slr.2 ignore dialogs without buttons&per.
.br
The :hp1.Steps :ehp1. spin button sets how many steps it takes to
move the mouse pointer to the destination&per.
.br
If you want to interrupt the mouse motion, move the pointer in the opposite
direction&semi. the mouse motion thread will detect that and will stop moving
the mouse&per.
:edl.
.br

.* htmlsource = pgmoukbd.htm
:h2 res=19 x=left y=bottom width=100% height=100%.The mouse&slr.keyboard functions dialog
.br
This dialog is present both as a notebook page and as an independent
dialog window&per. It will pop up each time that you click on a button to
select the keyboard or mouse event for a given option (for instance, the
:hp1.Mouse :ehp1. and :hp1.Keyboard :ehp1. buttons of the
:link reftype=hd res=13.Window rolling:elink.
page)&per.
.br
As an independent dialog, it has the ordinary :hp1.OK:ehp1. , :hp1.Cancel :ehp1.
and :hp1.Help :ehp1. buttons&per.
.br
Its purpose is to give the user a sort of summary of all the mouse
and keyboard events which have been selected&per.
:p.
The :hp1.Function&colon. :ehp1. list box shows all the available features&per.
:p.
The :hp1.Status bar :ehp1. shows the control which should detect the
chosen mouse&slr.keyboard event&per.
:p.
The :hp1.Enable :ehp1. checkbox is greyed out (unavailable) when the
given feature must be set from the appropriate checkbox on its own page&per.
:p.
When radio buttons for :hp1.Click :ehp1. and :hp1.Double click :ehp1.
are not displayed, you simply choose which mouse button and optional key
combination (:hp1.Ctrl :ehp1. and&slr.or :hp1.Alt :ehp1. and&slr.or
:hp1.Shift :ehp1.) will be used with that feature, e&per.g&per. :hp1.window frame
drag :ehp1. or :hp1.VIO clipboard copy :ehp1.&per.
:p.
The :hp1.Keyboard Mappings :ehp1. page (and the :hp1.Functions controlled
by the keyboard dialog :ehp1.) allows you to associate a function key (F1-F12)
plus :hp1.Insert :ehp1. , :hp1.Delete :ehp1. and :hp1.Backspace :ehp1.
with one of the modifier keys (:hp1.Ctrl :ehp1. , :hp1.Alt :ehp1. and
:hp1.Shift:ehp1.
) to perform one of the listed features&per.
:p.
:hp2.Note&colon.:ehp2.
:p.
Now it is possible to define keyboard accelerators for performing some
operations in the folder windows&colon. arrange icons, find, open parent, open
properties notebook, refresh, sort by creation date, sort by creation date,
sort by last access date, sort by last write date, sort by name, sort by
real name, sort by size, sort by type&per.
.br
:hp1.Sort by extension :ehp1. at the moment works only by activating
the corresponding feature of NPS WPS Enhancer (if installed)&per. In a future
release, this operation will be performed by a built-in routine&per.
.br
In Warp 4, :hp1.Ctrl-Ins :ehp1. lets you create a new file from the
clipboard contents&per. In a future release, this feature will be extended to
Warp 3&per.

.* htmlsource = pgwinkey.htm
:h2 res=20 x=left y=bottom width=100% height=100%.The Windows Keys functions dialog
.br
This dialog allows you to assign some useful functions to the :hp1.Windows
keys :ehp1. of your keyboard&per. The assigned functions are performed as soon
as the key is released&per.
:dl break=all.
.br
:dt.
:hp1.Do not perform any action:ehp1.
:dd.
This just disables the key&per.
:dt.
:hp1.Hide&slr.show all windows:ehp1.
:dd.
This hides&slr.unhides all windows visible in the current desktop
(just in case you are running a virtual desktop utility), allowing you to
access the desktop icons&per.
:dt.
:hp1.Roll&slr.unroll all windows:ehp1.
:dd.
This function rolls&slr.unrolls all windows visible in the current desktop
(just in case you are running a virtual desktop utility), enabling you to
access the desktop icons&per.
:dt.
:hp1.Show window list:ehp1.
:dd.
This function displays the window list at the current
mouse position&per.
:dt.
:hp1.Show context menu:ehp1.
:dd.
If the focus window has any context menu, it will pop up as the
selected key is released&per.
:dt.
:hp1.Lock up the desktop:ehp1.
:dd.
Locks the desktop like the corresponding item of the desktop popup menu&per.
:dt.
:hp1.Open drive folder:ehp1.
:dd.
This opens the drive folder&per. If the folder is already opened and
its :hp1.Object open behaviour :ehp1. (in the drive folder Properties Notebook)
is set to :hp1.Display existing window :ehp1., then the previously opened window
gets the focus and is brought to the top&per.
:dt.
:hp1.Open System Setup folder:ehp1.
:dd.
This opens the System Setup folder&per. If the folder is already opened and
its :hp1.Object open behaviour :ehp1.
(in the System Setup folder properties notebook)
is set to :hp1.Display existing window :ehp1., then the previously opened window
gets the focus and is brought to the top&per.
:dt.
:hp1.Open OS&slr.2 command window:ehp1.
:dd.
This opens an OS&slr.2 command window&per.
:dt.
:hp1.Show WarpCenter desktop menu:ehp1.
:dd.
This opens the Warp Center menu which displays the current desktop
content&per. This and the following features will work only if Warp Center
is currently running&per.
:dt.
:hp1.Show WarpCenter window list:ehp1.
:dd.
The Warp Center window list is displayed on the screen&per.
:dt.
:hp1.Show WarpCenter process list:ehp1.
:dd.
If the statement
.br
:font facename='System VIO' size=12x5.
SET KILLFEATUREENABLED=ON
:font facename=default size=0x0.
.br
is present in your CONFIG&per.SYS file, then this list will enable you to
kill processes&per.
:dt.
:hp1.Show WarpCenter tray list:ehp1.
:dd.
This function lists the Warp Center trays&per.
:dt.
:hp1.Switch to the next WarpCenter tray:ehp1.
:dd.
The next Warp Center tray is brought to the top as soon as the selected
key is released&per.
:dt.
:hp1.Show WarpCenter information menu:ehp1.
:dd.
This functions shows the content of the :hp1.Assistance center:ehp1.
folder as a Warp Center menu&per.
:edl.
.br

.* htmlsource = problems.htm
:h1 res=21 x=left y=bottom width=100% height=100%.Known problems
:ul.
.br
:li.
When you suspect that a particular program conflicts with Styler&slr.2, add
its name (including its full path) to the
:link reftype=hd res=8.General
Exceptions:elink.
page, then close the program (if it is still open) and restart
it&per. This usually works better than
:link reftype=hd res=6.disabling
Styler&slr.2:elink.
&per.
.br
If the program works correctly when listed in the General Exceptions page,
you can try to isolate the source of the conflict by deleting the program
pathname from the General Exceptions page and re-entering the pathname in
a list of local Exceptions for a given group of features&per. This approach lets you
fine-tune exceptions for a few particular options and avoid disabling
Styler&slr.2 globally for the program in question&per.
.br
As a last resort, if you do not succeed by adding the conflicting program
to the General Exceptions page nor by disabling Styler&slr.2, yet you want to
be sure that the problems do derive from Styler&slr.2, shutdown the system and
then reboot it&per. After the OS&slr.2 boot logo disappears, press the
:hp1.Control :ehp1. key and keep it pressed down until you hear a double
beep from the PC loudspeaker&per. This will avoid having to load SMARTWIN&per.DLL
during bootup and the system will behave as if the program never had been
installed&per.
.br
Now you can conduct all the tests needed to pinpoint the conflict&per. If test
results point to Styler&slr.2 as the cause, then
:link reftype=fn refid=29.send me:elink.
a report of the problem&per.
:li.
If you use Styler&slr.2 in combination with other PM enhancers like Xit, NPS
WPS Enhancer, Program Commander&slr.2, Object Desktop, etc&per., avoid conflicts
by disabling a given option under all but one enhancer&per.
:li.
The MPTS configuration program doesn&csq.t work properly with Styler&slr.2&per. The
window controls (rolling, close, etc) added by Styler&slr.2 seem
to be the cause of the problem&per.
.br
To solve this problem and let the program work properly, add MPTS&per.EXE
(including its full path) to the General Exceptions page or to the
list of exceptions for the particular features involved&colon.
:ol compact.
.br
:li.
titlebar exceptions (if you are using the titlebar enhancements)
:li.
titlebar hiding exceptions (only if you added the Hide Titlebar button
to the window controls)
:li.
window rolling (only if you added the Roll Windows button)&per.
:eol.
.br
:p.
:hp2.Note&colon.:ehp2.
.br
Since Styler&slr.2 version 1&per.1, the program MPTS&per.EXE is automatically added
to the exceptions by the installer&per.
:li.
Sometimes (it happened only a couple of times here) and for no apparent
reason, one of the mouse pointers in the current set (for instance, the :hp1.Size
NS :ehp1. pointer) gets corrupted&per. To get rid of the corrupted bitmap,
open the :hp1.Mouse:ehp1. object in the :hp1.System Setup:ehp1. folder, select
a different set of pointers and then restore the previous one through the
Undo  button&per.
:li.
The :hp1.View :ehp1. caption (i&per.e&per. Icon view, Tree view, etc&per.) is missing from
the folder titlebar&per.
:li.
The titlebar enhancements work only with real titlebars (FID_TITLEBAR windows)
so you get the default titlebar inside Pro-News&per.
:li.
Older releases of PMView (prior to PMView 2000) use their own procedure to paint
the titlebar&per. Upgrade to PMView 2000 if you want to get rid of this problem&per.
:li.
Some users reported that moving windows by dragging the titlebar did not always
work when Styler&slr.2 was installed along with Object Desktop&per. I modified
the mouse events detection procedure and the problem should have been reduced
if not completely eliminated&per. Another user suggested eliminating an Object
Desktop DLL from the list of the DLLs which are loaded during system startup&per.
.br
If you have Warp4 and installed fixpack 5 or a later one, you have
in your &bsl.OS2&bsl.SYSTEM directory the program REGEDIT2&per.EXE&semi. if you cannot find
it, you can find various free INI file editors on the Hobbes server&per. You
must edit your OS2&per.INI file, often defined as user profile or HINI_USER_PROFILE
in REGEDIT2&per.EXE, and look for the SYS_DLLS  item and its LoadPerProcess
sub-item&per.
.br
This sub-item contains a list of DLL names (not including the path
and the DLL extensions) separated by a space&per. To remove the Object Desktop
DLL from the list, delete the OBJDWCTL string, save the new INI file and
reboot&per.
.br
If the system works better this way and you are not missing any of
your favourite Object Desktop features, just leave it this way&per. If you
want to restore its previous state, just add the OBJDWCTL string to the
list&per. You could also try to change the order in the list by putting SMARTWIN
before OBJDWCTL&per.
.br
I do not own any version of Object Desktop and am not planning to buy
it just to make few tests, so I would be grateful if you let me know your
experiences with the above described modifications&per. If it works, I could
make a small application (and&slr.or add a setting in the Styler&slr.2 Preferences
Notebook) to automatically remove, restore or change the order of the Object
Desktop DLL in the list in OS2&per.INI&per.
:li.
Window rolling does not work properly with some programs (AICQ, MED)&per.
:li.
A user reported that in Warp 3, the color selection buttons do not work
when the mouse is set up for left-handed use&per. I tried unsuccessfully to
replicate the problem in a Warp 3 partition (refresh release - no fixpack)
so I guess it is some bug from one of the most recent fixpacks&per.
:li.
Window controls in Watch Cat do not work properly&per.
:li.
Various problems have been reported with Describe, OpenChat&slr.2&per.
:li.
The titlebar controls are not displayed properly with the IBM EWS application
PMCALC&per.
.br
If you check the :hp1.Context menu :ehp1. option in the Titlebar settings
page, you can access the system menu by clicking with mouse button 2
on the program titlebar&per.
:li.
The program has been only partially tested with Warp 3&per. I&csq.ve just installed
Warp 3 and soon will install the latest Warp 3 fixpack to check if everything
works properly&per. So Warp 3 compatibility should improve before the next
release&per.
:eul.
.br

.* htmlsource = planned.htm
:h1 res=22 x=left y=bottom width=100% height=100%.Planned features
.br
All the features planned for version 1 are present in this release&per.
.br
In future, only minor enhancements and bug fixes will be added to this
version&per.
.br
A preliminary list of the features planned for version 2 will be enclosed
in the first release of version 2&per.


:euserdoc.
